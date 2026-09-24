# Changelog

Six sections, and the distinctions between them are the point.

| § | Holds |
|---|---|
| 1 | Severe defects — data integrity, hardware safety, or process survival |
| 2 | Improvements, one subsection per theme |
| 3 | Documentation corrections |
| 4 | Hypotheses that were wrong |
| 5 | Known limitations — **deliberate scope, explicitly not defects** |
| 6 | Known open defects — confirmed and not yet fixed |

A confirmed-but-unfixed bug belongs in §6, never §5. The two invite opposite responses: §5 is a
boundary someone chose, §6 is work someone owes.

---

## 1. Severe defects

Nothing has been *fixed* yet — no code has been changed. The defects found by reading the tree
are recorded in §6, where they belong until a change closes them. Four of them are severe
enough that they are called out here as the reason Phase 0 and Phase 2 of `PLAN.md` are gates
rather than tidying:

- `getChanellIndex()` has an **empty body** and is called on every received frame
  (`src/Utils/Bitutils.cpp:34`). Its return value indexes a 4-element array. Every dispatch
  decision the receive path makes is therefore undefined behaviour reading an uninitialised
  register.
- The legacy wire format encodes **channel as one bit** (`getChanel` returns `x[6]`), so it can
  address 2 channels. `Instructions.md` item 11 requires 4 bits addressing 16. These are not
  reconcilable by refactoring; the instruction byte must be repacked. See §6.2.
- The receive thread **busy-spins at 100% CPU** — `dataReceivedTask()` loops `while(true)` over
  a non-blocking `read()` and `continue`s on `n<=0`, which is the normal no-data case.
- `DEI1016.cpp` does not compile: it references an undeclared `recData` and calls `push_back`
  on a type whose method is `push`.

---

## 2. Improvements

### 2.1 — Documentation set created (Phase 1)

`CLAUDE.md` amended from the OME++ text to this project; `PLAN.md`, `docs/markdowns/MAP.md`,
`DECISIONS.md`, `PRINCIPLES.md`, `WORKFLOW.md`, `CHANGELOG.md` and `PROGRESS.md` created.
Six design decisions (D1–D6) and eight principles (§1–§8) recorded before any core code was
written, which is the order the conventions require — a decision written down after the code
exists records a rationalisation, not a choice.

`tools/check_docs.py` added as a ctest target so the cross-references in that set are verified
mechanically rather than by care.

---

## 3. Documentation corrections

### 3.1 — `LABEL_BYTE` and `ARINC_BYTE0` both naming index 5 is correct, not a conflict

An earlier draft of `DECISIONS.md` D4 and `WORKFLOW.md` §1 flagged the shared index 5 as
"cannot both be right if the payload is 8 contiguous bytes". **That was wrong.** In ARINC 429
the label *is* bits 1–8 of the 32-bit word, so the low ARINC byte and the label byte are the
same byte by definition. `convertBytesToData` merging `ARINC_BYTE0..3` into the 32-bit word is
consistent with this. Both entries corrected; the aliasing is intentional and is now documented
as such rather than as an open question.

### 3.2 — `CLAUDE.md` described a different project

The committed `CLAUDE.md` was the working-conventions file for "OME++", a multi-symbol order
matching engine with a SQLite ledger, gRPC intake and `ome_*` metrics. None of its paths
(`docs/markdowns/OME++_*.md`, `src/Metrics.cpp`, `tests/tsan_grpc.supp`, `bench/`, `sql/`,
`deploy/`) exist in this repository. The five conventions were sound and project-independent and
were kept; every path, metric and domain example was repointed or removed, and the provenance is
noted at the top of the amended file so nobody mistakes it for having always described this
project.

### 3.3 — `README` carries unresolved merge markers

`README` is unmerged in the working tree and contains `<<<<<<< HEAD` / `=======` / `>>>>>>>`
markers. Content, not a formatting nit. Fixed in Phase 0.

---

## 4. Hypotheses that were wrong

### 4.1 — "The legacy code is a working reference implementation"

`Instructions.md` item 20 says `DEI1016.h` "contains a clean code for the UsbGate and reading a
DataPack", and the plan was written on that basis — harvest the parser, refactor it into
`UsbGate`, move on.

Reading `DEI1016.cpp` and `Bitutils.cpp` contradicts it. The channel-index decoder that the
entire 16-channel dispatch depends on is an empty function body. The file does not compile. The
receive loop busy-spins. The header declares `std::array<SpscRingBuffer<record_t, 64>, 4>` while
the `.cpp` pushes into a differently-shaped buffer, and the `record_t` it pushes has an
uninitialised byte 0.

**What this changes:** `DEI1016` is a reference for *intent and wire framing*, not for
implementation. `UsbGate` gets written against the frame layout and the state machine's shape,
with its own tests, rather than being refactored in place and assumed correct because it was
already there. `PLAN.md` Phase 3 says "refactor"; that wording stands, but the exit criterion —
a loopback test that proves dispatch and proves out-of-range refusal — is what carries the
weight, not the refactor.

The lesson that generalises: a specification's characterisation of existing code is a claim
about that code, and it is checked by reading the code. `PRINCIPLES.md` §8 exists for cost
claims; this is the same failure in a different costume.

### 4.2 — "The instruction byte has room for a 4-bit channel index"

It does not. `Bitutils.cpp` packs the byte as bits 0–3 instruction, bit 4 transmit/receive, bit
5 DEI, bit 6 channel, bit 7 unused. Only bit 7 is free. Encoding a 0–15 channel index needs 4
bits, and taking them means shrinking the instruction field or the DEI/transmit-receive flags,
or using bit 7 plus three bits from elsewhere. This is a **wire format change**, so it is a new
decision citing D4, made in Phase 2 — not something to discover while writing `UsbGate`.

---

## 5. Known limitations — deliberate scope, explicitly not defects

- **No SPMC buffer exists or is needed.** `Instructions.md` item 5 lists SPMC among the buffer
  families; the design uses none, because each channel has exactly one consumer
  (`DECISIONS.md` D3). A passive channel monitor would require one. Deliberate.
- **`Tests/` is not a suite.** Loose `.cpp` files, a `build.sh`, and a committed `a.out`. Not
  wired to ctest, not run automatically. Replaced in Phase 2; until then there is no automated
  test coverage of anything, which is a limitation of the current state rather than a defect in
  a component.
- **`GeneralTester.pro` (qmake) and `CMakeLists.txt` coexist and disagree.** Keeping both is not
  a decision; it is an undecided state, tracked as open question Q1 in `PLAN.md`. Once Q1 is
  answered the loser is deleted.
- **The legacy naming convention is not retro-fitted.** `CLAUDE.md` §0's snake_case /
  camelCase / PascalCase rule binds new code. Legacy symbols (`getChanellIndex`, `chanel_index`,
  `bIfSerialOpen`, `setControlWord_receiver_32Bits`) are left alone; renaming them is its own
  change on its own branch because it touches callers.
- **20% wire overhead from framing.** Two of every ten bytes carry no data. Accepted in
  `DECISIONS.md` D4 as the price of resynchronisation after a dropped byte.
- **POSIX-only transport.** `Bitutils.cpp` includes `<termios.h>` and `DEI1016::openPort` opens
  `/dev/ttyAMA0` or `/dev/ttyS1` under `#ifdef RASBERRYPI` / `ORANGEPI`. There is no Windows
  serial path, so the core cannot be exercised on the development machine without the loopback
  transport of `PRINCIPLES.md` §5. That is the reason §5 is a principle and not a nice-to-have.

---

## 6. Known defects, open

Ordered by how much they block. Each names the file, the evidence, and the phase that owns it.

### 6.1 — `getChanellIndex()` has an empty body  ·  `src/Utils/Bitutils.cpp:34`

```cpp
uint16_t getChanellIndex(record_t& recData) {

}
```

No return statement in a non-void function. Called at `src/Hardware/DEI1016.cpp:202` on every
successfully parsed frame, and its result indexes `receive_data_buffer`, a
`std::array<SpscRingBuffer<record_t, 64>, 4>`. Reading the uninitialised return is UB; using it
as an index makes the out-of-bounds case reachable on roughly 3 of every 4 values.

This is the function `Instructions.md` item 13 depends on — "The UsbGate dispatches each pocket
of data to the corresponding Channel through channel_index". **It has never worked.** Owned by
Phase 2 (correct decode) and Phase 3 (its use in dispatch).

### 6.2 — Channel is 1 bit in the wire format; the spec needs 4  ·  `src/Utils/Bitutils.cpp:24,57`

`getChanel` returns `x[6]` — a single bit, so 0 or 1. `setChanel` assigns `x[6] = ch`,
truncating any value to one bit. `Instructions.md` item 11 requires the first 4 bits to be a
channel index 0–15, and item 4 requires 16 channels.

The instruction byte is fully committed except bit 7 (see §4.2). Resolving this is a wire format
change requiring a new decision citing D4. **Blocks Phase 2**, because the framing helpers
cannot be written until the layout is settled, and blocks Phase 3 because `UsbGate` dispatches on
it.

### 6.3 — `DEI1016.cpp` does not compile  ·  lines 199–203

- `recData` is used at lines 199, 200, 201 and is **not declared** anywhere. `DEI1016.h:64` has
  only a commented-out `// QByteArray recDataBuffer;`.
- `receive_data_buffer[chanel_index].push_back(...)` at line 203 — `SpscRingBuffer` has `push`,
  not `push_back`.
- `chanel_index` is not declared in `DEI1016.h` either (the header declares `counter`, `state`,
  `receive_data`).
- `AUX::log(recData, ...)` — the free `log(QByteArray&, str_t)` at line 104 is not in namespace
  `AUX`, and takes a `QByteArray`, which `receive_data` (a `record_t`) is not.
- `#include "BitUtils.h"` at `Bitutils.cpp:1` — the real file is `Bitutils.h`. Case-insensitive
  on Windows, a hard error on the Raspberry Pi target.

Consequence: **the committed state of the receive path has never been built.** This is why
`PLAN.md` Phase 0's exit criterion is "it configures and builds", and why §4.1 revised the
"harvest the reference implementation" assumption.

### 6.4 — Receive thread busy-spins at 100% CPU  ·  `src/Hardware/DEI1016.cpp:122`

```cpp
while(true) {
    int n = ::read(fd, &bytes, FRAME_POCKET_SIZE);
    if (n<=0){ continue; }
```

The port is opened `O_NONBLOCK` (`openPort`), so `read` returns `-1`/`EAGAIN` whenever no data
has arrived — the common case. `continue` re-reads immediately. No sleep, no `poll`, no
`select`. One core pegged for the lifetime of the application, on a Raspberry Pi, while also
starving the very thread that would consume the data.

Owned by Phase 3. `WORKFLOW.md` §4's open question Q3 (Qt event loop vs `std::jthread`) is
precisely the choice of what replaces this loop.

### 6.5 — `receive_data[0]` is never written, then pushed as part of a frame  ·  `DEI1016.cpp:168–203`

`counter` starts at 1 and the parser fills indices 1–8 only; index 0 (the start marker) is
consumed by the state machine and never stored. `recData[0] = 255` shows the author knew a
marker belonged there — but `receive_data`, not `recData`, is what gets pushed to the channel
buffer. So index 0 of every dispatched frame is whatever was in that member previously: stale
data from the last frame, or uninitialised memory on the first.

Any consumer reading the frame's start byte, or treating indices 0–9 as the canonical layout,
gets garbage. Related to 6.3 (`recData` vs `receive_data` looks like a rename that was applied
half-way). Owned by Phase 3.

### 6.6 — Partial reads are silently discarded  ·  `DEI1016.cpp:131`

`::read(fd, &bytes, FRAME_POCKET_SIZE)` may legally return fewer than 10 bytes. `n` is used as
the loop bound, so a short read is parsed as if complete and the parser state carries across
calls — which is fine — but the `State` machine's `counter` is a *member* while `state` is
reassigned to `WaitForInitial` at the top of `dataReceivedTask`, so re-entering the function
mid-frame desynchronises it. With the busy-spin of 6.4 this is a live path, not a theoretical
one.

The frame markers exist to make this recoverable (`DECISIONS.md` D4); the recovery is what needs
testing. Owned by Phase 3, and it is the case the loopback test must cover explicitly.

### 6.7 — `Event.h` does not compile  ·  `src/Utils/Event.h:55`

`using ControlEventPtr = std::unique_ptr<ControlEven>;` — `ControlEven`, missing the trailing
`t`. Also: all constructors are `protected` or `= delete`, so no `Event` can be constructed from
outside the hierarchy, and `toPacket()` returns a bare `char*` with no length.

Superseded rather than fixed — `DECISIONS.md` D5 overwrites the class in Phase 2. Recorded here
because it is evidence for §4.1 and because any legacy caller breaks at compile time when the
overwrite lands.

### 6.8 — `CMakeLists.txt` does not match the tree

Lists sources that do not exist: `src/Utils/utils.cpp`, `src/Items/baseitem.cpp`,
`src/Items/baseitem.h`, `src/Workers/transmitterworker.cpp/.h`, `src/Workers/receiverworker.cpp/.h`,
`src/Widgets/settingsdialog.cpp/.h/.ui`, `src/Items/Arinc429/equipmentsids.cpp/.h`,
`src/Items/Arinc429/Labels/labelfor.cpp/.h`, `src/Widgets/dialog.ui`, `src/Utils/utils.h`,
`src/Workers/DataSourceBase.h`. Uses lowercase names for `PascalCase` files (`event.h`,
`types.h`, `singleton.h`, `bitutils.h`, `generaldata.h`, `spsc_ring_buffer.hpp`,
`mpmc_ring_buffer.hpp`, `receiver.h`, `transmitter.h`, `mainwindow.h`, `checkboxarray.h`,
`editordelegate.h`). The real files are `Event.h`, `Types.h`, `Singleton.h`, `Bitutils.h`,
`Generaldata.h`, `SpscRingBuffer.hpp`, `MpmcRingBuffer.hpp`, etc. Also references
`src/Utils/Reactor.h`, `ThreadedBuffer.h`, `PointerVector.h` inconsistently with what is listed.

Case mismatches build on Windows and fail on Linux — which is how this survived. **Blocks
everything**; owned by Phase 0.

### 6.9 — `Singleton.h` ignores its index parameter

```cpp
template<typename T> class Singleton {
    static T* getInstance(int index) {
        static std::unique_ptr<T> instance;
        if (!instance) { instance = std::make_unique<T>(nullptr); }
        return instance.get();
    }
};
```

`index` is accepted and unused: every index returns the same single instance. `SingletonArray`
above it is correct. A caller asking for instance 3 and silently receiving instance 0 is the
exact failure `PRINCIPLES.md` §3 exists to prevent. Also `getInstance` is not thread-safe in the
`if (!instance)` check pre-C++11-style — in practice the function-local `static` initialisation
is thread-safe in C++11+, but the surrounding `if` is dead code that suggests otherwise.

Owned by Phase 5, which uses this header for `InstanceData`.

### 6.10 — `MpmcRingBuffer.hpp` has no `#pragma once` and no includes

The file begins directly with `template <typename T, size_t N> class MpmcRingBuffer`, using
`std::atomic`, `std::array` and `size_t` with no `#include <atomic>`, `<array>` or
`<cstddef>`. It compiles only because its includer happened to pull those in first — which is a
build-order dependency, not a header. No include guard either, so double inclusion is a redefinition
error.

Owned by Phase 2, which is where the buffers get their tests and therefore where this surfaces.

### 6.11 — `SpscRingBuffer`'s `pop()` returns by value into a `T` copy

`T item = buffer_[r % N]; rc_.store(r + 1, ...)` publishes the slot as free *after* the copy,
which is correct — but the copy is made from a slot the producer may be concurrently
overwriting only if the producer's full-check is wrong. The producer's `w - r == N` test is
correct for this design, so the buffer is sound **for its documented topology**. Recorded here
rather than in §5 because the soundness depends entirely on `PRINCIPLES.md` §1 holding, and a
future second consumer turns a correct buffer into a data race with no code change in this file.

### 6.12 — Working tree is mid-merge

`MERGE_HEAD` present (`4aafb70`). Unmerged: `README`, `menus/action.cpp`, `menus/action.h`,
`menus/menus.pro.user`, `src/Widgets/menus.pro.user.1528e23`. Also `.git/AUTO_MERGE` exists.
`git status` cannot be clean until this is resolved, so no change can be committed as an
isolated, reviewable unit.

Owned by Phase 0. It is the first thing in `PLAN.md` for this reason.
