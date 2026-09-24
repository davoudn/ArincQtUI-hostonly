# PLAN.md — the route from here to the event-based core

This is the phase text and the exit criteria. Whether each phase met its criterion lives in
`docs/markdowns/PROGRESS.md` — the two are deliberately separate, because a plan rewritten
to match reality is a worse record than one that says where it missed (see `CLAUDE.md` §3).

`Instructions.md` (workspace root, one directory up) is the specification. The six design
questions it left open are answered in `docs/markdowns/DECISIONS.md` as **D1–D6** and
summarised below.

---

## Current state, plainly

Two blockers sit in front of all feature work. They are Phase 0 and they are a **hard gate**:
nothing after them can be *verified* until they are done, and an unverifiable change is not a
finished change.

1. **The repository is mid-merge.** `MERGE_HEAD` is present. Unmerged paths: `README`,
   `menus/action.cpp`, `menus/action.h`, `menus/menus.pro.user`,
   `src/Widgets/menus.pro.user.1528e23`. `README` still contains conflict markers.
2. **The build does not configure.** `CMakeLists.txt` lists sources that are not in the tree
   (`src/Utils/utils.cpp`, `src/Items/baseitem.cpp`, `src/Workers/transmitterworker.cpp`,
   `src/Workers/receiverworker.cpp`, `src/Widgets/settingsdialog.cpp`,
   `src/Items/Arinc429/equipmentsids.cpp`, plus several headers and `.ui` files) and uses
   lowercase names where the real files are `PascalCase` (`spsc_ring_buffer.hpp` vs
   `SpscRingBuffer.hpp`, `event.h` vs `Event.h`, `types.h` vs `Types.h`). It pins
   `CMAKE_CXX_STANDARD 17`. A second, disagreeing build system (`GeneralTester.pro`) is also
   checked in.

Neither is described in `Instructions.md`; both were found by reading the tree.

---

## Locked decisions (full text in `DECISIONS.md`)

| | Decision | One line |
|---|---|---|
| **D1** | C++20 | Bump `CMAKE_CXX_STANDARD` 17 → 20; Qt6 + pybind11 support it. |
| **D2** | UsbGate TX is MPMC | 16 channel producers → 1 USB consumer; reuse `MpmcRingBuffer`, size power-of-two. |
| **D3** | Channel RX is SPSC | UsbGate thread sole producer, `runProcedure` sole consumer; one procedure at a time makes SPSC sound. |
| **D4** | 10-byte frame is the wire/buffer unit | `record_t = array<uint8_t,10>`; the 8-byte `DataPacketType` is the payload inside it. |
| **D5** | `Event` is rewritten | The existing `src/Utils/Event.h` does not compile; overwrite it to spec. |
| **D6** | `runProcedure()` | Name of the member that runs a channel's procedure and enforces one-at-a-time. |

---

## Open questions (not decisions yet — do not assume an answer)

- **Q1 — CMake or qmake?** Both are checked in and disagree. One survives, the other is
  deleted. Becomes a numbered decision when answered. *Recommendation: CMake*, because
  sanitizer build trees, ctest and a pybind11 module target are all natural in it and awkward
  in qmake.
- **Q2 — MPMC buffer depth.** `MpmcRingBuffer` requires a power-of-two `N`. The TX depth under
  worst-case 16-channel burst is unmeasured. Pick a value in Phase 3 and record the reasoning;
  a full TX buffer drops packets (see `CLAUDE.md` §6, "name what you could not do").
- **Q3 — Which thread runs the USB RX parse loop?** `DEI1016` uses a `QThread` + `QTimer` +
  `QSerialPort` today. Whether `UsbGate` keeps Qt's event loop or runs a bare `std::jthread`
  affects how `Channel` procedures wait for responses. Decide in Phase 3.
- **Q4 — pybind11 module name and packaging.** Affects Phase 6 CMake wiring and how Python
  scripts import it. Decide in Phase 6.
- **Q5 — Non-blocking workflow handle type.** Item 18 case 3 ("outcome checked later, timeout
  still enforced"). A `std::future`, a pollable handle object, or a Qt signal each have
  different pybind11 ergonomics. Decide in Phase 4.

---

## Phases

### Phase 0 — Unblock the repo  *(hard gate)*

Resolve the merge (finish or abort it deliberately — record which), clear the `README`
markers, repair `CMakeLists.txt` against the real tree (per Q1), answer Q1, bump to C++20,
add the `BT_SANITIZE` build option and an `enable_testing()`/ctest skeleton.

**Exit criterion:** from a clean checkout, `cmake -S . -B build && cmake --build build`
succeeds; `git status` reports no unmerged paths and no conflict markers anywhere; the chosen
build system is the only one left.

### Phase 1 — Documentation set

Already substantially delivered (this file plus `CLAUDE.md` and `docs/markdowns/`). Remaining:
write `tools/check_docs.py`, wire it into ctest, add the "Start here" block to `README.md`.

**Exit criterion:** `check_docs.py` runs green as a ctest target; every `§` reference in the
doc set resolves; D1–D6 are in `DECISIONS.md` with rows in `MAP.md`.

### Phase 2 — Core types and buffers

Create `src/Core/`. New `Event` (D5), framing helpers turning a `record_t` into a
`DataPacketType` view and back (D4), `channel_index` decode from the high nibble. Unit-test
both existing ring buffers as the foundation. Replace the loose `Tests/` files with one test
binary per suite.

**Exit criterion:** buffer, framing and `channel_index`-decode tests pass under
**ThreadSanitizer with no first-party suppression**; framing round-trips a packet
byte-identically.

### Phase 3 — UsbGate

Refactor `DEI1016` into `UsbGate` in `src/Core/`: owns the MPMC TX buffer (D2) and one USB
worker thread; the RX parse loop decodes `channel_index` and pushes directly into the target
Channel's SPSC RX buffer (Instructions items 13–14). Put the serial transport behind an
interface so a loopback/mock backend exists; the core must be testable with no board attached.
Answer Q2 and Q3.

**Exit criterion:** a loopback test moves a packet from `UsbGate` TX into the correct
Channel's RX buffer, and a malformed/out-of-range `channel_index` is refused and counted, not
mis-delivered to channel 0; clean under TSan.

### Phase 4 — Channel

`Channel`: own SPSC RX buffer (D3), a `procedure` slot (`std::function`), and `runProcedure()`
(D6) enforcing one-at-a-time. The three workflows from Instructions item 18:
1. fire-and-forget send;
2. blocking send-and-wait-for-specific-response with timeout;
3. non-blocking variant of 2 — outcome checked later, timeout still enforced via a deadline
   in the worker loop. Answer Q5.

**Exit criterion:** simulated-response tests cover all three workflows **including both timeout
paths** (response-never-arrives and response-arrives-late); a second procedure submitted while
one runs is refused or queued by an explicit, documented rule — not silently dropped.

### Phase 5 — InstanceData

Singleton (reuse `src/Utils/Singleton.h`) holding `std::array<Channel,16>` and the `UsbGate`
reference. Correct startup ordering and **clean shutdown** — every thread joins, ownership
ends in reverse order of construction. The legacy `DEI1016` singleton does not do this;
`InstanceData` must.

**Exit criterion:** repeated startup/shutdown cycles leak nothing and hang nothing under
**both ASan/UBSan and TSan**; the 16 channels are addressable by index and out-of-range access
throws rather than defaults.

### Phase 6 — pybind11 bindings

Create `bindings/`. Expose InstanceData access, per-channel `sendEvent`, the three workflow
calls, and event/packet introspection. Blocking workflow calls **release the GIL**; the
non-blocking handle maps to a Python future or pollable object (per Q5). Add the pybind11
module as a CMake target beside the Qt app. Answer Q4.

**Exit criterion:** a Python script, with no C++ test harness, drives a send-and-wait
round-trip over the loopback gate and observes both a success and a timeout.

### Phase 7 — Integration and UI

Connect the new core to the Qt widgets. The legacy `src/Workers/` event handlers are replaced
by Channel procedures. Decide per-widget whether it drives the core directly or through Python.

**Exit criterion:** the UI transmits and receives through the new core with no legacy
`Workers/` handler on the data path.

### Phase 8 — Validation

Full verification protocol (`CLAUDE.md` §4): build + ctest + ASan/UBSan tree + TSan tree, then
hardware-in-the-loop against the real DEI1016 board through the usb-gate.

**Exit criterion:** all suites green except failures explicitly recorded as expected in
`CHANGELOG.md`; the HIL pass is recorded in `PROGRESS.md` with the board and firmware
identified.

---

## Sequencing note

Phases are ordered by dependency, not by size. Phase 0 gates everything because nothing can be
verified until the build configures. Phase 1 gates the rest because the record is where the
open questions get answered and a question answered in a chat does not survive to the next
session. Phases 2–5 are the core and can each be reviewed against its own exit criterion
independently. Phase 6 needs 2–5. Phases 7–8 need everything.
