# Decisions

Numbered, permanent, never renumbered. A later decision **cites** an earlier one rather than
reopening it. Each entry names: the choice, the alternative rejected, the cost accepted, and
what would reopen it.

These six answer the questions `Instructions.md` left open. They were settled in conversation
on 2026-09-17 and are recorded here so the next session inherits them instead of re-deriving
them.

| | Decision | Full text | Bears on it |
|---|---|---|---|
| **D1** | C++20, not C++17 | §1 | principle §6, §8 |
| **D2** | UsbGate TX ring is MPMC | §2 | principle §1, §8 · wf §3 |
| **D3** | Channel RX ring is SPSC, and no SPMC is needed | §3 | principle §1, §4 · wf §3 |
| **D4** | The 10-byte frame is the unit; the 8-byte packet is a view into it | §4 | principle §2, §3 · wf §1, §2 |
| **D5** | `Event` is overwritten, not repaired | §5 | principle §2 · wf §2 |
| **D6** | The procedure runner is `runProcedure()` | §6 | principle §1, §7 · wf §5, §6 |

---

## D1 — The project is C++20

**The decision:** `CMAKE_CXX_STANDARD` moves from 17 to 20.

**Rejected:** staying on C++17. `Instructions.md` item 1 allows either ("C++17 or 20"), so
this was a genuine choice rather than a constraint. C++20 was chosen because the core is
concurrency-shaped and the standard library support for it is materially better: `std::jthread`
gives join-on-destruct, which is exactly the clean-shutdown property `PRINCIPLES.md` §6
requires and which the legacy `DEI1016` singleton lacks; `<bit>` and `std::bit_cast` make the
frame/payload reinterpretation in D4 explicit and defined rather than a `reinterpret_cast`
through a `char*`, which is what `Event::toPacket()` does today and is undefined behaviour for
a non-trivial type; and `std::atomic::wait`/`notify_one` would let a blocking workflow (D6,
workflow 2) sleep on an arrival instead of spinning, without introducing a condition variable.

**Cost:** the toolchain must support C++20 on every target — the development machine (Windows,
MSVC or MinGW) and the Raspberry Pi target the legacy `DEI1016RasberryConfigurations` files
imply. Older GCC on an embedded image is the realistic risk. Qt 6 requires C++17 as a floor
and supports C++20; pybind11 supports C++20. Neither is the constraint.

**Revisit if:** the Raspberry Pi deployment image cannot be upgraded to a C++20-capable
compiler. If that happens, the core must be written to the C++17 intersection and D1 is
superseded by a new decision citing it — not edited.

---

## D2 — The UsbGate transmit ring is MPMC

**The decision:** `UsbGate` owns one `MpmcRingBuffer<record_t, N>` for transmit. All 16
channels push into it; one USB worker thread drains it.

**Rejected:**
- *A dedicated MPSC buffer.* `Instructions.md` item 7 says "mpsp", which reads as MPSC, and
  MPSC is the exact topology — many producers, one consumer. It was rejected because a
  correct MPSC implementation is strictly more subtle than the MPMC one already sitting in
  `src/Utils/MpmcRingBuffer.hpp`, and an MPMC buffer is a valid MPSC buffer: using it with one
  consumer simply never exercises the consumer-side CAS contention. Writing a new lock-free
  structure to save a few nanoseconds on a path gated by USB transfer time is the wrong trade.
- *Sixteen per-channel TX buffers drained round-robin.* Rejected because it moves the
  arbitration policy into `UsbGate` and makes fairness a scheduling question instead of a
  queueing one, and because it multiplies by sixteen the number of buffers that must be
  reasoned about under TSan.

**Cost:** the MPMC `push` does a CAS loop where an MPSC would do a single atomic
fetch-add. Contention on the USB transmit path is expected to make this indistinguishable from
zero — USB is the bottleneck — but that is an expectation, not a result. Per `CLAUDE.md` §5 it
gets measured, with spread, before it is asserted. The cost that is real and cannot be measured
away: `MpmcRingBuffer` requires `N` to be a power of two (`static_assert`), so the depth is
quantised. That is open question **Q2** in `PLAN.md`.

**Revisit if:** a benchmark shows TX-ring contention is measurable against the USB transfer
cost, or if a per-channel transmit rate limit is ever required — which round-robin over
sixteen buffers would give for free and a single shared MPMC ring would not.

---

## D3 — Each Channel's receive ring is SPSC, and no SPMC buffer is needed

**The decision:** each of the 16 channels owns one `SpscRingBuffer<record_t, 64>`. The
`UsbGate` RX thread is the sole producer; the thread running that channel's procedure is the
sole consumer.

`Instructions.md` item 5 lists "spsc or spmc or mpmc" as the buffer families in play. SPMC is
**not** used anywhere, and that is deliberate rather than an omission.

**Rejected:**
- *An SPMC channel RX buffer.* It would be needed if several consumers read one channel's
  incoming events. They do not: item 17 says at each instant only one procedure runs for a
  channel, and the procedure is what consumes. One consumer, structurally, forever — as long
  as item 17 holds. An SPMC buffer would pay a CAS on every pop to support a concurrency the
  design forbids.
- *A mutex-protected `std::queue`.* Rejected because it puts a lock on the path from the USB
  RX thread into every channel, and that lock is held by whichever channel's consumer happens
  to be slowest. The point of per-channel buffers is that a stalled channel stalls itself, not
  the dispatcher.

**Cost:** SPSC correctness is a **claim about the topology**, not a property of the type. The
type cannot enforce it. If anything ever produces into a channel's RX buffer from a second
thread, or consumes from two, the buffer is silently corrupt — no assertion fires, the data is
just wrong. That is why `PRINCIPLES.md` §1 makes the topology the documented, reviewed thing.
The depth is fixed at 64 in the existing declaration; a full buffer drops the packet, and D4
plus `CLAUDE.md` §6 require the drop be counted and named.

**Revisit if:** a channel ever needs more than one consumer — for example a passive monitor
observing a channel without disturbing the running procedure. That is a plausible feature and
it would require a new decision citing this one, plus a buffer that supports it.

---

## D4 — The 10-byte frame is the unit of buffering and transmission; the 8-byte packet is a view into it

**The decision:** everything buffered and everything on the wire is a `record_t`, which is
`std::array<uint8_t, 10>`. `DataPacketType`, which is `std::array<uint8_t, 8>`, is the payload
read out of a frame. It is never transmitted instead of one.

This resolves an apparent contradiction in `Instructions.md`: item 8 defines the data field as
fixed 64 bits written as "(8*8 bytes)" — the specification's own arithmetic is loose here; 64
bits is 8 bytes, and `8*8` would be 64 *bytes* — while the legacy code in `src/Utils/Types.h`
defines `FRAME_POCKET_SIZE 10` and `record_t` as `std::array<uint8_t,10>`. The owner confirmed
on 2026-09-17 that the frame byte was simply missing from the specification. **`Instructions.md`
should be amended to say so**; that amendment is tracked in `PROGRESS.md`.

The frame layout, read off the index macros in `Types.h`:

| Index | Macro | Holds |
|---|---|---|
| 0 | `INITIAL_BYTE_INDEX` | start-of-frame marker (`0xFF`) |
| 1 | `INSTRUCTION_BYTE` | instruction code (see `enum Instructions`) |
| 2 | `RATE_BYTE` | rate |
| 3–4 | `CONTROL_BYTE0`, `CONTROL_BYTE1` | control word (16-bit) |
| 5 | `LABEL_BYTE` / `ARINC_BYTE0` | label, and low ARINC byte |
| 6 | `ARINC_BYTE1` | ARINC data |
| 7 | `ARINC_BYTE2` | ARINC data |
| 8 | `ARINC_BYTE3` | ARINC data |
| 9 | `FINAL_BYTE_INDEX` | end-of-frame marker (`0xFF`) |

This table is **read off the macros, not off a working parser.** `Types.h` declares
`INITIAL_BYTE 0xFF` twice and defines `DATA_POCKET_SIZE 8` beside `FRAME_POCKET_SIZE 10`.
`LABEL_BYTE` and `ARINC_BYTE0` both naming index 5 is **correct, not a conflict** — in ARINC 429
the label *is* bits 1–8 of the 32-bit word, so the low ARINC byte and the label byte are the
same byte by definition, and `AUX::convertBytesToData` merging `ARINC_BYTE0..3` into the word
is consistent with that. (An earlier draft of this section called the aliasing an open question;
it was wrong, and the correction is recorded in `CHANGELOG.md` §3.1.)

Phase 2 must still confirm the table against a *working* parser — which does not exist yet, see
`CHANGELOG.md` §6.1 and §6.3. Where the code and this table disagree once the parser is written,
the code wins and the table is corrected, with the correction recorded in `CHANGELOG.md` §3
rather than silently edited.

**A wire-format change is required and is not settled here.** The legacy instruction byte is
packed as bits 0–3 instruction, bit 4 transmit/receive, bit 5 DEI, bit 6 channel, bit 7 unused
(`src/Utils/Bitutils.cpp`). **Channel is one bit**, addressing 2 channels, where this decision
and `Instructions.md` item 11 require 4 bits addressing 16. Bit 7 alone is free. Repacking it
means shrinking the instruction field, the DEI flag, or the transmit/receive flag, or splitting
the index across bytes. That is a successor decision to D4, made in Phase 2, and it is tracked
as `CHANGELOG.md` §6.2. This decision fixes the *frame length and the framing rationale*; it
does not fix the bit layout inside byte 1.

**Rejected:**
- *Buffering the bare 8-byte payload.* Smaller, and what item 8 read literally would give.
  Rejected because the frame markers are what let the RX parser resynchronise after a partial
  read or a dropped byte on a serial line. Without them a single lost byte desynchronises the
  stream permanently and every subsequent packet is misparsed into the wrong channel. That
  failure is silent, and for a test instrument it is catastrophic.
- *A variable-length framed packet.* Rejected because every buffer here is a fixed-size ring of
  trivially-copyable elements; variable length means either a max-size slot (wasting the common
  case) or out-of-band heap storage (destroying the lock-free property).

**Cost:** two bytes in ten — 20% wire overhead — carry no data. At `BAUD_RATE 921600` this is
irrelevant next to the framing's value. The real cost is that every piece of code touching a
packet must know whether it holds a frame or a payload, and confusing them is an off-by-two bug
that reads as corrupt data rather than as a crash. `PRINCIPLES.md` §2 makes that a named rule
for exactly this reason.

**Revisit if:** the wire protocol changes — a different board, or a move from framed serial to
bulk USB transfers with their own packetisation. Either is a new decision citing this one.

---

## D5 — `Event` is overwritten, not repaired

**The decision:** `src/Utils/Event.h` is replaced with a new `Event` written to the
specification. The existing file is a reference for intent only.

The existing file does not compile and cannot be incrementally fixed:
- line 55 declares `ControlEventPtr = std::unique_ptr<ControlEven>` — `ControlEven`, a typo for
  a type that does not exist;
- `BaseEvent`'s constructors are in a `protected:` section while being the only way to
  construct the class, and the default constructor is `= delete`; `DataEvent` and
  `ControlEvent` delete their default constructors too;
- `toPacket()` returns a raw `char*` with no size, so the caller must already know it is 10
  bytes — the frame length of D4 encoded as tribal knowledge;
- the class carries `dei`, `channel`, `tran_receive`, `instruction`, `rate` as separate
  `uint32_t`/`float` members and reassembles them into a packet on demand, duplicating state
  the frame already holds;
- the members are `protected` in `BaseEvent` but the subclasses declare their own fields
  `protected` too, so nothing outside the hierarchy can read an event it is given.

**Rejected:**
- *Repairing it in place.* Cheaper in diff size, and it would preserve the `DataEvent` /
  `ControlEvent` split, which is a reasonable distinction. Rejected because the duplication
  above is the design problem, not the typo: an `Event` holding fields *and* a serialisation of
  them has two sources of truth and they will disagree. The owner's instruction on 2026-09-17
  was to overwrite.
- *Keeping `DataEvent`/`ControlEvent` as subclasses.* **Deferred, not rejected.** The new
  `Event` holds a `record_t` and a direction; whether the data/control distinction becomes a
  subclass, an enum, or simply the `INSTRUCTION_BYTE` of D4's frame is a Phase 2 design choice
  to be made by looking at how `enum Instructions` is actually consumed. It is not settled by
  this decision.

**Cost:** any legacy caller of `DataEvent` or `ControlEvent` breaks at compile time. That is the
point — the breakage is the inventory of what must be updated. `git grep` both names in Phase 2
before deleting anything.

**Revisit if:** the data/control distinction turns out to need genuinely different behaviour,
not just a different instruction byte — then a hierarchy is justified and this decision gets a
successor citing it.

---

## D6 — The procedure runner is `runProcedure()`

**The decision:** the member function that runs a channel's procedure is `runProcedure()`.
`Instructions.md` item 16 leaves its sentence unfinished ("The actual name of member function
which runs those procedure could run, there are ."), and item 17 states the invariant it must
enforce: at each instant only one procedure runs for a channel.

The owner confirmed `runProcedure` on 2026-09-17.

`runProcedure()` owns three things:
1. taking the `procedure` slot (a `std::function` / lambda per item 16) and running it;
2. enforcing one-at-a-time — a second submission while one runs is **refused or queued by an
   explicit, documented rule**, and which is chosen is a Phase 4 decision recorded here as a
   successor to D6, not left implicit;
3. being the sole consumer of that channel's SPSC RX buffer, which is what makes D3 sound.

Point 3 is the load-bearing one. D3's SPSC claim holds *because* `runProcedure` is the only
consumer. A future change letting a procedure hand its RX buffer to a helper thread breaks D3
without touching a single line of buffer code.

**Rejected:** `execute()`, `run()`, `dispatch()`. `runProcedure` was chosen because the
specification's own vocabulary is "procedure", and a name reusing the domain word survives a
reader who has read `Instructions.md` but not the headers.

**Cost:** none material. Naming is free now and expensive later.

**Revisit if:** item 16's unfinished sentence turns out to have specified more than a name — a
family of runners, say. Then this decision is superseded, not amended.
