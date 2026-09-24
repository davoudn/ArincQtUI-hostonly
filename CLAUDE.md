# Working in this repository

**ArincQtUI-hostonly** is a Qt/C++ host application for ARINC 429 test equipment, talking
to a DEI1016 board over a serial/USB gate. It is being restructured around an event-based
core: 16 bidirectional channels, lock-free ring buffers, and a Python scripting surface via
pybind11.

`Instructions.md` (one directory up, at the workspace root) is the specification of that
core. `PLAN.md` is the phased route to it. This file is about **how work is done here**, and
it exists because several of these conventions are load-bearing and none of them are
guessable from the code.

The short version: **documentation is part of the deliverable, not a follow-up.** A
change is finished when it is written into the project's own record, not when it
compiles.

> **Provenance.** This file was amended for this project from a `CLAUDE.md` that described a
> different codebase ("OME++", an order matching engine). The conventions were kept because
> they are good and project-independent; every path, metric and domain example was repointed
> or removed. Where a convention names a tool that does not exist here yet, it says so
> inline rather than implying it works. See `docs/markdowns/CHANGELOG.md` §3.

---

## 0. Naming, stated once

From `Instructions.md` item 1, and binding on all new code:

| Element | Convention | Example |
|---|---|---|
| Class / type | `PascalCase` | `UsbGate`, `Channel`, `InstanceData` |
| Member function | `camelCase` | `runProcedure()`, `sendEvent()` |
| Member variable | `snake_case` | `channel_index`, `rx_buffer` |

Legacy code predates this and is **not** retro-fitted as a side effect of another change.
Renaming a legacy symbol is its own change, on its own branch, because it touches callers.
Known deviations are listed in `docs/markdowns/CHANGELOG.md` §5.

---

## 1. Branch first, always

Create a branch for each distinct piece of work before starting it:

```
git checkout -b <topic>
```

**Being on a branch that is not `main` is not a reason to treat it as the right home
for whatever comes next.** This rule exists because a documentation set was once
committed onto an unrelated topic branch simply because that branch happened to be
checked out, and it landed in the middle of work it had nothing to do with.

Commit to the topic branch and **leave it local**. Pushing is asked for explicitly;
do not push unprompted.

**The repository is currently mid-merge** (`MERGE_HEAD` is present; `README`,
`menus/action.cpp`, `menus/action.h`, `menus/menus.pro.user` and
`src/Widgets/menus.pro.user.1528e23` are unmerged). Until that is resolved, `git status`
will not be clean no matter what you do, and a new branch starts from an unfinished
state. Resolving it is Phase 0 of `PLAN.md` and comes before any feature work.

---

## 2. Every change updates the documentation set

Five documents carry different things, and they are not interchangeable. All live in
`docs/markdowns/`.

### `MAP.md` — the reverse index

Coordinates only, no arguments: one row per subsystem giving its theory section, its
decision, its principle, its workflow section and the code it landed in. **It is the
file to open first**, and the only one that answers "I am looking at this code, what
is its story" rather than requiring you to already know which principle to look up.

A new principle, decision or workflow section **adds a row here in the same change**.
And never move an argument into it — the moment a row grows a paragraph it becomes a
sixth document to keep in sync and stops being a map.

### `CHANGELOG.md` — what changed, and what it cost or fixed

The sections have distinct meanings and must be respected:

| § | Holds |
|---|---|
| 1 | Severe defects — data integrity, hardware safety, or process survival |
| 2 | Improvements, one numbered subsection per phase or theme |
| 3 | Documentation corrections |
| 4 | Hypotheses that were wrong |
| 5 | Known limitations — **deliberate scope, explicitly not defects** |
| 6 | Known open defects |

A confirmed-but-unfixed bug goes in §6, never §5.

### `WORKFLOW.md` — how the mechanism actually works

Thread map, buffer topology, and the three procedure workflows. Flow diagrams carry the
document's own `[T: thread-name]` markers. Update the **neighbouring** sections a change
touches, not only the new one: a change to the framing of a packet belongs in the wire-format
section as well as its own, and a new cross-thread mechanism belongs in the thread map too.

### `PRINCIPLES.md` — the reasoning that outlives both

**This document is a link in the decision chain, not a citation target.** The chain
runs principles → decision → code, and it runs in both directions: existing
principles constrain what gets built, and what gets built *extends* the principles
for the next change.

So when a change establishes a new invariant, or resolves a design tension in a way
future work should inherit, that reasoning belongs here as a numbered principle **in
the same change that introduces it**. A change that only cites principles has
consumed the chain without extending it. Add the summary-map row at the bottom too,
and the row in `MAP.md`.

### `PROGRESS.md` — the live state of the plan

One entry per slice, newest last, each naming its commit. **A phase moves to done
only when its exit criterion passes as written** — not when its code exists. Read
this file first when picking work up again, and update it in the same commit as the
work.

`PLAN.md` at the repo root holds the phase text and the exit criteria themselves;
`PROGRESS.md` holds whether they were met.

### And it is checked, not merely asked for

`tools/check_docs.py` is **not written yet** — it is scheduled in `PLAN.md` Phase 1 and
tracked in `PROGRESS.md`. When it exists it will verify only mechanically falsifiable
claims, so every failure is actionable and never a matter of taste:

1. every `§` cross-reference resolves to a heading that exists
2. every principle has a summary-map row **and** a row in `MAP.md`
3. every file path named in the docs is real
4. any number the docs quote that can be recomputed from source, is

Until then, run the equivalent by hand before committing a documentation change:

```
git grep -n '§' -- '*.md' docs/markdowns PLAN.md
```

The worked example of this checker found 23 genuine dangling references on its first run
in the project it came from — twenty of them left by a file rename that had gone unnoticed
for months. **A reference that no longer resolves is the one failure this whole
documentation set cannot survive quietly.**

### And the thing most easily forgotten

**The navigational note at the top of a document goes stale too.** These files open
with a summary of what has been built and where to find each phase's outcome. Adding
a body section without updating that opening map leaves the document introducing
itself as an earlier version of itself. Treat the top-of-file note as part of every
documentation change, not as boilerplate.

---

## 2a. The conventions, in portable form

`.claude/skills/project-record/SKILL.md` is this way of working distilled and made
generic — the five artifacts, the eight conventions, the templates, and a kickoff
prompt. It exists so the approach can be carried to another project rather than
living only in this repository's habits, and it is the honest short version of what
this file spells out at length.

It also states the thing that explains the shape and was unwritten here for months:
**these documents are not a record of the thinking, they are where the thinking
happens.** A decision gets made by being written down, because the writing is what
forces the rejected alternative to be named.

---

## 3. Plans are never rewritten to match reality

`PLAN.md` phase text is **not edited to match what happened**. Deviations get a
new "what X turned out to be" section instead, and struck-through text is preferred
over deletion.

A plan quietly rewritten to match reality is a worse record than one that says where
it missed. Follow this rather than tidying.

The same honesty applies to exit criteria: if a clause reads "loopback test moves a
packet UsbGate→correct Channel, **clean under ThreadSanitizer**", then a passing test
with no TSan run is `PARTIAL`, not `PASS`. A delivery record that overstates a clause
is worse than one that omits it, because the omission gets noticed.

---

## 4. Verification protocol

**The build does not currently configure** — `CMakeLists.txt` names source files that are
not in the tree and uses lowercase filenames where the real ones are `PascalCase`. That is
`PLAN.md` Phase 0. The protocol below is what applies once it is fixed; do not skip it
because the first run is red.

Run all three before committing, not at the end of a phase:

```
cmake -S . -B build && cmake --build build -j
```

```
cd build && ctest --output-on-failure
```

Then the sanitizers, which are separate build trees:

```
cmake -S . -B build-asan -DBT_SANITIZE=address,undefined -DCMAKE_BUILD_TYPE=Debug
```

```
cmake -S . -B build-tsan -DBT_SANITIZE=thread -DCMAKE_BUILD_TYPE=Debug
```

(`BT_SANITIZE` does not exist in `CMakeLists.txt` yet; it is added in Phase 0 alongside the
source-list repair. Sanitizer flags are a build-system change, not a per-run choice.)

Anything touching a thread, a ring buffer or shared mutable state must be clean under
**ThreadSanitizer with no first-party suppression**. The ring buffers are the foundation
of the whole design — a data race there is not a local defect, it is a corrupted event
stream sixteen channels downstream.

`Tests/` today is loose `.cpp` files with a `build.sh` and a committed `a.out`. Those are
not a suite and are not wired to ctest. Phase 2 replaces them with one test binary per
suite, so a crash takes down only its own suite and TSan can be pointed at just the
concurrency suites.

---

## 5. Cost claims are measured, never asserted

`bench/` does not exist yet. It holds one benchmark per claim when it does, and a phase
that adds work to a hot path carries a gate. Two rules learned the hard way elsewhere and
adopted here up front:

- **Report the spread, not just the number.** A benchmark reporting single runs once
  had every mode of a feature coming out *faster* than the baseline it is a strict
  superset of. That is not a result; it is a harness whose noise floor exceeds its
  effect. Print a median and the observed spread beside it, so "no measurable cost"
  stays distinguishable from "we did not look hard enough". For a lock-free buffer this
  matters doubly: MPMC throughput is contention-dependent, and a single-threaded
  measurement of an MPMC buffer measures nothing that will be true in production.
- **A gate can be overtaken.** If building the thing a gate measures would itself
  violate the property the gate was protecting, say so, satisfy the property, and
  measure the part that genuinely lands on the hot path — then record why the reading
  differs from the one the plan asked for.

---

## 6. House style in the code

Comments explain **why**, and specifically what the alternative was and why it was
rejected. The reasoning that outlives a function lives in `PRINCIPLES.md`; the reasoning
an editor of *this* class needs stays in its header.

Recurring rules:

- **Concurrency safety comes from topology, not from locks.** A buffer is SPSC because
  exactly one thread produces and exactly one consumes — that is a claim about the design,
  and it is what makes the lock-free implementation correct. If a change would add a second
  producer, it must change the buffer type, not add a mutex around it. See `PRINCIPLES.md` §1.
- **The frame is the unit; the payload is a view.** Everything buffered and everything on
  the wire is a 10-byte `record_t`. The 8-byte `DataPacketType` is read out of it, never
  sent instead of it. See `DECISIONS.md` D4.
- **Refuse rather than default.** An out-of-range `channel_index`, an unrecognised
  instruction, a zero timeout — refuse it where it is read, count it, and say so. A bad
  nibble that silently becomes channel 0 delivers data to the wrong equipment.
- **Name what you could not do.** Dropped packets on a full ring buffer, unpriced
  timeouts, frames that failed to parse and channels with no consumer are all reported
  explicitly, because "empty" and "we stopped looking" are opposite claims. A silently
  dropped packet on a full buffer is the failure mode this design is most exposed to.
- **Clean shutdown is part of the design.** Every thread joins; ownership ends in reverse
  order of construction. The legacy `DEI1016` singleton does not do this and the new
  `InstanceData` must. See `PRINCIPLES.md` §6.
- **Hardware sits behind an interface.** The core is testable with a loopback backend and
  no board attached. See `PRINCIPLES.md` §5.

---

## 7. Layout

| Path | Holds |
|---|---|
| `src/Core/` | **The new event core** — `Event`, `Channel`, `UsbGate`, `InstanceData`. Created in Phase 2. Does not exist yet. |
| `src/Utils/` | Ring buffers (`SpscRingBuffer.hpp`, `MpmcRingBuffer.hpp`), `Singleton.h`, `Types.h`, bit and timer helpers. |
| `src/Hardware/` | `DEI1016` — the reference implementation `UsbGate` is refactored from. |
| `src/Items/Arinc429/` | ARINC 429 data model: labels, parameters (BNR/BCD/FLNO/…), equipment. |
| `src/Widgets/` | Qt UI: `MainWindow`, `Transmitter`, `Receiver`, dialogs, `.ui` files. |
| `src/Workers/` | Legacy event handlers, superseded by Channel procedures in Phase 7. |
| `bindings/` | **pybind11 module.** Created in Phase 6. Does not exist yet. |
| `Tests/` | Currently loose files and a `build.sh`. Becomes one binary per suite in Phase 2. |
| `docs/markdowns/` | The record. See §2. |
| `tools/` | `check_docs.py` when written. Does not exist yet. |
| `CustomIcons/LabelsData/`, `Icons/` | Label data JSON and Qt resources. |

Two build systems are checked in — `CMakeLists.txt` and `GeneralTester.pro` (qmake). They
disagree, and only one should survive. That choice is **open** and is listed under Open
Questions in `PLAN.md`; it is not recorded as a decision until it is made.
