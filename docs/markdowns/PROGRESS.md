# Progress — the live state

One entry per slice, newest last, each naming its commit. A phase is **done** only when its
exit criterion passes **as written** (`CLAUDE.md` §3). `PARTIAL` is not `PASS`.

Read this file first when picking work up again. Update it in the same commit as the work.

The phase text and the exit criteria live in `PLAN.md`; this file records whether they were met.

---

| Phase | State | Exit criterion status |
|---|---|---|
| 0 — Unblock the repo | not started | — |
| 1 — Documentation set | **PARTIAL** | docs written; `check_docs.py` wired to ctest = pending |
| 2 — Core types and buffers | not started (blocked by 0, and by §6.2) | — |
| 3 — UsbGate | not started | — |
| 4 — Channel | not started | — |
| 5 — InstanceData | not started | — |
| 6 — pybind11 bindings | not started | — |
| 7 — Integration and UI | not started | — |
| 8 — Validation | not started | — |

---

### Slice 1 — The documentation set · PARTIAL · commit: *(uncommitted)*

**Built:** `CLAUDE.md` amended from the OME++ text to this project. `PLAN.md`,
`docs/markdowns/MAP.md`, `DECISIONS.md`, `PRINCIPLES.md`, `WORKFLOW.md`, `CHANGELOG.md`,
`PROGRESS.md` created. `tools/check_docs.py` written. Six decisions (D1–D6) and eight principles
(§1–§8) recorded. `Instructions.md` amended in place with a dated addendum rather than edited —
the specification is a record too, and rewriting it to match what we learned would destroy the
evidence of what was ambiguous.

**What surprised us:** the specification's characterisation of the existing code is not accurate,
and the gap is large. `Instructions.md` item 20 calls `DEI1016.h` "a clean code for the UsbGate
and reading a DataPack". The channel-index decoder that all 16-channel dispatch depends on —
`AUX::getChanellIndex` — is an **empty function body**, called on every received frame
(`CHANGELOG.md` §6.1). `DEI1016.cpp` does not compile: undeclared `recData`, `push_back` on a
type whose method is `push`, a case-mismatched include that breaks on Linux (§6.3). The receive
thread busy-spins at 100% CPU on a non-blocking fd (§6.4). Twelve defects recorded, four severe.

The consequence is a change of approach, recorded as a wrong hypothesis in `CHANGELOG.md` §4.1:
`DEI1016` is a reference for **intent and wire framing**, not a working implementation to
refactor and trust. Phase 3's exit criterion — a loopback test proving dispatch and proving
out-of-range refusal — carries the weight that "it was already there" was going to carry.

Second surprise, and it blocks Phase 2: the legacy wire format encodes channel as **one bit**
(`getChanel` returns `x[6]`), addressing 2 channels. Item 11 requires 4 bits for 16. The
instruction byte is fully committed except bit 7, so this is a genuine repacking decision —
shrinking the instruction field, the DEI flag, or the transmit/receive flag — not a refactor.
`CHANGELOG.md` §6.2, §4.2. It needs an owner decision before framing helpers can be written.

**Exit clauses:** the document set exists and its cross-references resolve — `check_docs.py`
passes. **Not met:** it is not yet a ctest target, because there is no working build to attach it
to (§6.8, Phase 0). The `README` "Start here" block is written but `README` is still unmerged,
so it cannot be committed. Hence `PARTIAL`.

**Verified:** `python tools/check_docs.py` green (28 cross-references resolved, 0 dangling). No
build, no tests — the build does not configure (§6.8). Nothing here is a code change, so nothing
needed sanitizers.

---

## Blocked, and on what

| Blocked | By | Unblocks when |
|---|---|---|
| Every phase | mid-merge working tree (§6.12) | Phase 0 resolves it |
| Every phase | `CMakeLists.txt` does not match the tree (§6.8) | Phase 0 repairs it |
| Phase 2 | channel-index bit layout undecided (§6.2) | an owner decision, recorded as a successor to D4 |
| Phase 2 | `getChanellIndex` empty (§6.1) | written, with tests |
| Phase 1 exit | no ctest to wire `check_docs.py` into | Phase 0 |

The Phase 2 blocker is **not** a coding task — it is a question for the owner. Bit 7 is free;
bits 0–6 are taken by instruction (4), transmit/receive (1) and DEI (1). Sixteen channels need
four bits. Something gives, and which thing gives is a hardware-protocol decision, not an
implementation detail. It is listed in `PLAN.md` as an open question and it should be answered
before Phase 2 starts, because the framing helpers and their tests both depend on it.

## Not started, deliberately

No code has been changed. `src/` is exactly as committed, defects included. That is the correct
state for a slice whose deliverable is the record: writing the core before the wire format is
settled would produce code that has to be rewritten when §6.2 is answered, and the rewrite would
be invisible in this file because the first version was never recorded as wrong.
