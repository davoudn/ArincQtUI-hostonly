# Principles

Numbered, and **a link in the decision chain, not a citation target**. The chain runs
principles → decision → code, and it runs in both directions: existing principles constrain
what gets built, and what gets built *extends* them for the next change.

A change that only cites a principle has consumed the chain without extending it. When a change
establishes a new invariant, add it here as a numbered principle **in the same commit**, with a
row in the summary map at the bottom and a row in `MAP.md`.

The first eight were written before the core exists. They are the constraints the design was
reasoned under, drawn from `Instructions.md` and from what reading the legacy code showed.
Principles earned by building — the ones that only become visible when something breaks — come
later and are numbered on from §8.

---

## §1 — Concurrency safety comes from topology, not from locks

A buffer is SPSC because exactly one thread produces into it and exactly one consumes. That is
a claim about the *design*, and it is the claim that makes the lock-free implementation
correct. The type cannot check it. Nothing at compile time or run time will complain when it is
violated; the data is simply wrong afterwards.

Therefore the topology is the documented, reviewed artifact. Any change that would add a second
producer or a second consumer to a buffer **changes the buffer type**; it does not add a mutex
around the existing one. A mutex bolted onto a lock-free structure to fix a topology mistake
keeps the mistake, hides it, and adds a lock on a hot path — the worst of all three.

This is why D3 can rest on item 17 of the specification ("only one procedure runs at a time")
rather than on anything in `SpscRingBuffer.hpp`. The invariant lives in `Channel`, and
`runProcedure` is its enforcer.

*Consequence for review:* a diff that adds a thread anywhere in `src/Core/` must state which
buffers that thread touches and in which role. "It just reads" is not an answer — reading is
consuming.

---

## §2 — The frame is the unit; the payload is a view

Everything buffered and everything transmitted is a 10-byte `record_t`. The 8-byte
`DataPacketType` is read out of a frame and written back into one. It never travels alone (D4).

Code that handles a packet must therefore always know which of the two it is holding, and a
function that can take either is a function with an off-by-two bug waiting in it. Prefer
distinct types over a `record_t` that is "sometimes a frame": the compiler enforces what the
type system can express, and a comment cannot.

The frame markers are not decoration. They are the resynchronisation mechanism that lets the RX
parser recover from a dropped byte on a serial line instead of misparsing every subsequent
packet into the wrong channel — silently, which is the part that matters.

---

## §3 — Refuse rather than default

An out-of-range `channel_index`, an unrecognised instruction, a zero timeout, a frame whose
markers do not match: refuse it where it is read, count it, and say so.

The specific danger here is that `channel_index` is a 4-bit field decoded from packet data, so
it is *always* in range 0–15 as an integer and always looks valid. The refusal cases are the
structural ones — bad frame markers, a truncated read, an instruction the channel does not
handle. A bad frame that is accepted anyway does not fail loudly; it delivers data to whichever
channel its garbage nibble names.

The general rule behind it: **a misspelling or a corrupt byte that silently becomes a default
leaves the equipment running a policy nobody chose.** For test hardware driving avionics bus
labels, that is the failure that reaches a customer.

---

## §4 — Name what you could not do

Truncation, dropped packets, refused procedures, expired timeouts and frames that failed to
parse are all reported explicitly, because "empty" and "we stopped looking" are opposite claims
and only one of them is true.

A full ring buffer drops its packet. That is the correct behaviour — blocking the USB RX thread
on one slow channel would stall all sixteen — but the drop must be counted and the count must
be readable. An instrument that reports "no data received" when the real answer is "data
received and discarded because the consumer was 64 packets behind" sends the operator looking
at the wiring instead of at the procedure.

This applies to the documentation too: a phase that meets four of five exit clauses is
`PARTIAL`, and `PROGRESS.md` says `PARTIAL`.

---

## §5 — Hardware sits behind an interface

The core is testable with no board attached. Every path through `UsbGate`, `Channel` and
`InstanceData` runs against a loopback transport in tests; the real DEI1016 is one
implementation of a transport interface, not a dependency of the design.

Two reasons, and the second is the stronger one:
1. hardware is scarce, slow and not present in CI;
2. **the interesting failures in this design are concurrency failures**, and those need
   thousands of repetitions under ThreadSanitizer to surface. A test that requires a physical
   board runs once, by hand, by whoever has the board. A loopback test runs on every commit
   under TSan. The race that corrupts a channel at one-in-a-million packets will be found by
   the second and never by the first.

The legacy `DEI1016` is a `QObject` singleton holding a `QSerialPort` directly, so its logic
cannot be exercised without the port. Refactoring it into `UsbGate` (Phase 3) is where this
principle gets paid for, and where the seam must be cut.

---

## §6 — Clean shutdown is part of the design, not a cleanup step

Every thread joins. Ownership ends in reverse order of construction: channels stop accepting
procedures before the gate stops feeding them, and the gate stops before the buffers it writes
into are destroyed.

The legacy `DEI1016` singleton does not do this — it holds a `QThread` member and a raw
`instance` pointer with no documented teardown, and a singleton that is never destroyed simply
never has to answer the question. `InstanceData` will have to, because Phase 5's exit criterion
is repeated startup/shutdown cycles clean under ASan and TSan, and a test that cannot tear down
cannot be repeated.

The reason this is a principle and not a task: **a thread still running when its buffer is
destroyed does not fail at shutdown, it fails at some later unrelated moment** — or in the next
run of a test suite, where it looks like flakiness in whatever test happened to be scheduled
after it.

---

## §7 — A timeout is a property of the request, not of the waiting call

`Instructions.md` item 18 case 3 asks for a non-blocking send-and-wait in which "the outcome
will be checked later, but the timeout is still there even the way the method is ran is not
blocking." That is the harder half of the requirement and the easy half to lose.

So: the deadline is established when the request is made and travels with it. It is not a
parameter of the blocking wait, and it is not reset by checking the outcome. A non-blocking
request that nobody polls still expires; a caller that polls late gets "timed out", not "still
waiting".

The alternative — timeout as an argument to a blocking `wait()` — makes the non-blocking
variant a different feature with different semantics, and the two would drift. One deadline
model, two ways of consuming it.

---

## §8 — Cost claims are measured, and report their spread

A performance claim without a median and an observed spread is not a claim. This is
`CLAUDE.md` §5 restated as a principle because it constrains design decisions, not just
write-ups: D2 accepts MPMC's CAS loop over MPSC's fetch-add on the *reasoned* grounds that USB
transfer time dominates. That reasoning is plausible and unmeasured, and it stays labelled as
reasoning until `bench/` exists.

The rule that makes it bite: a benchmark whose noise floor exceeds its effect will eventually
show a strict superset of work running *faster* than its baseline. When that happens the
harness is wrong, not the result surprising — and reporting it as a result is how a bad number
enters a design document and gets cited for a year.

---

## Summary map

| § | Principle | One line | Decisions citing it |
|---|---|---|---|
| 1 | Topology, not locks | SPSC is a design claim the type cannot check | D2, D3 |
| 2 | Frame is the unit | 10-byte `record_t` on the wire, 8-byte payload as a view | D4, D5 |
| 3 | Refuse rather than default | A corrupt byte must not become channel 0 | D4 |
| 4 | Name what you could not do | Dropped ≠ absent; `PARTIAL` ≠ `PASS` | D2, D3, D6 |
| 5 | Hardware behind an interface | Loopback transport, so races are testable | D1, D5 |
| 6 | Clean shutdown is design | Threads join; teardown in reverse construction order | D1 |
| 7 | Timeout belongs to the request | The deadline travels; polling late still yields "timed out" | D3, D6 |
| 8 | Cost claims are measured | Median and spread, or it is reasoning not a result | D1, D2 |
