# Workflow — how the mechanism runs

This is the one piece of *reference* documentation in the set: what calls what, on which
thread, in what order. It is the only document a reader might read start to finish.

**Status warning, and it matters.** Phases 2–8 of `PLAN.md` are not built. Everything in §2
onward below describes the **intended** mechanism as specified by `Instructions.md` and
constrained by `DECISIONS.md` — not observed behaviour. Sections describing code that exists
today are marked *(legacy, real)*. When a phase lands, its sections are updated in the same
commit and the marking removed. A workflow document that describes an unbuilt system in the
present tense is the fastest way to make a plan unreadable as a plan.

Thread markers use `[T: name]`. Buffer roles use `[P]` producer and `[C]` consumer, because per
`PRINCIPLES.md` §1 the topology *is* the correctness argument.

---

## §1 — The wire format  *(legacy, real — `src/Utils/Types.h`)*

Ten bytes on the wire, ten bytes in every buffer (D4):

```
 0    1           2      3      4       5      6      7      8      9
+----+-----------+------+------+-------+------+------+------+------+----+
|0xFF|instruction| rate |ctl_lo|ctl_hi |label |arinc1|arinc2|arinc3|0xFF|
+----+-----------+------+------+-------+------+------+------+------+----+
 start                                          ARINC 429 word      end
```

Macro names and the caveat that this table is read off the macros, not off a working parser,
are in `DECISIONS.md` §4. `LABEL_BYTE` and `ARINC_BYTE0` sharing index 5 is correct — in ARINC
429 the label is bits 1–8 of the 32-bit word, so they are the same byte.

**Open, and it blocks Phase 2:** the legacy instruction byte encodes channel as **one bit**
(bit 6), addressing 2 channels, where `Instructions.md` item 11 needs 4 bits for 16. Only bit 7
is free. Repacking is a wire-format change and a successor decision to D4; see
`CHANGELOG.md` §6.2 and §4.2. The diagram above shows the *intended* layout, not the legacy one.

The 8-byte `DataPacketType` is a view over indices 1–8. `channel_index` is the high nibble of
`DataPacketType[0]`, giving 0–15, which is the 16 channels of `Instructions.md` item 11 — and is
therefore dependent on the repacking above.

---

## §2 — `Event`  *(planned, Phase 2)*

A value type carrying a `record_t`, a direction (transmit / receive), and the decoded
`channel_index`. It replaces the non-compiling `src/Utils/Event.h` (D5).

Whether the legacy `DataEvent` / `ControlEvent` split survives as subclasses, as an enum, or as
nothing beyond the frame's `instruction` byte is **not decided** — D5 explicitly defers it to
Phase 2. This section is updated when it is.

---

## §3 — Buffer topology  *(SPSC/MPMC types legacy and real; the topology planned, Phases 3–4)*

```
                          [T: channel-procedure ×16]
                          one thread per channel,
                          runs one procedure at a time (item 17)
                                   |
                          push()   |   [P] ×16
                                   v
                    +--------------------------------+
                    |  UsbGate  tx_ring (MPMC)       |   D2
                    |  MpmcRingBuffer<record_t, N>   |
                    +--------------------------------+
                                   |
                                   |  pop()  [C] ×1
                                   v
                          [T: usb-worker]  ---- write ---->  USB / serial  --->  DEI1016
                                   ^                                                |
                                   |                                                |
                                   +<------------- read ----------------------------+
                                   |
                          parse frame, decode channel_index
                                   |
                          push()   |   [P] ×1   (UsbGate is the ONLY producer)
                                   v
        +----------------+  +----------------+       +----------------+
        | Channel 0      |  | Channel 1      |  ...  | Channel 15     |
        | rx SPSC<64>    |  | rx SPSC<64>    |       | rx SPSC<64>    |   D3
        +----------------+  +----------------+       +----------------+
                |                   |                        |
                |  pop() [C] ×1     |                        |
                v                   v                        v
          runProcedure()      runProcedure()           runProcedure()
          [T: channel-procedure 0]  ...                [T: channel-procedure 15]
```

The two invariants the whole design rests on, restated where an editor of this file will see
them:

- **`tx_ring` has 16 producers and 1 consumer.** MPMC is required (D2).
- **Each `rx` ring has 1 producer — the `usb-worker` — and 1 consumer — that channel's
  `runProcedure`.** SPSC is sound *only* while both halves hold (§3 of `DECISIONS.md`,
  `PRINCIPLES.md` §1). UsbGate pushes directly into the target channel's buffer
  (`Instructions.md` items 13–14); there is no intermediate dispatch queue.

`N` for the TX ring is open question **Q2**. RX depth is 64, from the existing
`std::array<SpscRingBuffer<record_t, 64>, 4>` declaration in `DEI1016.h` — note the legacy
array is **4** channels, and the new design is **16**.

---

## §4 — Transport  *(legacy `DEI1016` real; the seam planned, Phase 3)*

The legacy path: `DEI1016::openPort` / `configurePort(BAUD_RATE 921600)` on a `QSerialPort`, a
`QThread mainThread` member, a `QTimer serialResetTimer` at `SERIAL_RESET_INTERVAL 100` ms, and
a `parse(uint8_t byte)` state machine walking `State { WaitForInitial, InitialReceived,
WaitForFinal, FinalReceived }`.

The planned path: `UsbGate` holds a transport interface, with the serial implementation and a
loopback implementation behind it. `PRINCIPLES.md` §5 is why — the concurrency failures this
design is exposed to need thousands of TSan repetitions, and a board gives you one manual run.

Open question **Q3**: whether `UsbGate` keeps Qt's event loop (`QThread` + `QTimer` +
`QSerialPort`, as the legacy code does) or runs a bare `std::jthread`. This decides how a
blocking workflow waits, so it must be settled before Phase 4.

---

## §5 — Threads  *(planned, Phases 3–5)*

| Thread | Count | Owns | Produces into | Consumes from |
|---|---|---|---|---|
| `[T: usb-worker]` | 1 | the serial/USB handle, the frame parser | 16 × channel `rx` | `tx_ring` |
| `[T: channel-procedure i]` | 16 | channel *i*'s `procedure` slot | `tx_ring` | channel *i*'s `rx` |
| `[T: main / Qt GUI]` | 1 | the UI | — | — |
| `[T: python]` | 1+ | the interpreter, the GIL | via the bindings | via the bindings |

17 core threads plus the GUI plus Python. `[T: main]` never touches a ring buffer directly —
it goes through a channel's procedure or through the bindings, so the GUI cannot become an
unmodelled producer (§1 of `PRINCIPLES.md`).

Whether each channel needs a *dedicated* thread or whether channels share a worker pool is
**not settled** and is implied by Q3. `Instructions.md` item 17 requires only that a channel
runs one procedure at a time; a pool satisfies that with a per-channel claim, and 16 mostly-idle
threads is a real cost on a Raspberry Pi. This is flagged here so it is not silently decided by
whoever writes `Channel` first.

`InstanceData` (Phase 5) owns construction and destruction of all of the above, and teardown
runs in reverse: channels stop accepting procedures → the gate stops draining → the gate stops
producing → buffers are destroyed (`PRINCIPLES.md` §6).

---

## §6 — The three workflows  *(planned, Phase 4)*

All three are entered through `runProcedure()` (D6), which enforces one-at-a-time per channel.

**1 — Fire and forget.** Build a frame, `tx_ring.push()`, return. No consumption of `rx`.
If the push fails the buffer is full, and per `PRINCIPLES.md` §4 that is reported, not
swallowed.

**2 — Blocking send-and-wait.** Push the request frame, then consume this channel's `rx` until
a frame matching the expected response arrives or the deadline passes. Returns the response or
a timeout. This is the sole consumer of `rx` for the duration, which is what keeps §3's SPSC
claim true.

**3 — Non-blocking send-and-wait.** Push the request, register the deadline and the matcher,
return a handle immediately. The deadline is established **here**, at request time, and travels
with the request: a handle polled after the deadline yields "timed out", never "still waiting"
(`PRINCIPLES.md` §7). Somebody must still be draining `rx` for the matcher to run — which is a
design obligation on the non-blocking path, not an optional extra, and it is the part most
likely to be gotten wrong.

The handle type is open question **Q5**; a `std::future`, a pollable object and a Qt signal have
different pybind11 ergonomics (§8).

What happens to a response that arrives for a channel whose procedure already timed out and
returned is **not specified** in `Instructions.md`. It is a real case — the late response of
`PRINCIPLES.md` §7 — and Phase 4 must decide and record it, because "discard", "queue for the
next procedure" and "surface as an error" are three different behaviours and all three are
defensible.

---

## §7 — Shutdown  *(planned, Phase 5)*

Reverse of construction. The failure this prevents is specific: a thread still running when its
buffer is destroyed does not fail at shutdown, it fails later and elsewhere, and in a test suite
it presents as flakiness in whatever ran next (`PRINCIPLES.md` §6). The legacy `DEI1016`
singleton never answers this question because it is never destroyed.

---

## §8 — Python boundary  *(planned, Phase 6)*

`[T: python]` enters through pybind11 and reaches `InstanceData`, per-channel `sendEvent`, and
the three workflows. Two rules carry over from the thread map:

- **A blocking workflow call releases the GIL.** Holding it while waiting on a deadline would
  freeze the interpreter for the timeout duration and make workflow 2 unusable from Python.
- **Python is never a ring-buffer producer.** It calls into a channel, and the channel's
  procedure thread does the pushing. If a binding pushes to `tx_ring` directly from
  `[T: python]`, the producer count of §3 becomes 17 and the topology documented in §5 is a lie
  — which is exactly the silent violation `PRINCIPLES.md` §1 warns about.

Workflow 3's handle maps to a Python future or pollable object per Q5.
