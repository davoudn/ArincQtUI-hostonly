# The Map — open a file, find its story

This document contains no arguments, only coordinates. Every other document is a
forward index on its own axis; this is the reverse one.

A `§n` refers to a numbered section in the named document. `Dn` is a decision,
`wf §n` a section of `WORKFLOW.md`. Paths marked *(planned)* do not exist yet — the phase
that creates them is named in the last column. Unmarked paths are real.

| Subsystem | Decision | Principle | Mechanism | Code | Created by |
|---|---|---|---|---|---|
| Wire frame (10-byte `record_t`) | D4 | §2 | wf §1 | `src/Utils/Types.h` | exists (legacy) |
| `DataPacketType` (8-byte payload view) | D4 | §2 | wf §1 | `src/Core/DataPacket.h` *(planned)* | Phase 2 |
| `channel_index` decode | D4 | §3 | wf §2 | `src/Core/DataPacket.h` *(planned)* | Phase 2 |
| `Event` | D5 | §2 | wf §2 | `src/Utils/Event.h` → overwritten | Phase 2 |
| SPSC ring buffer | D3 | §1 | wf §3 | `src/Utils/SpscRingBuffer.hpp` | exists |
| MPMC ring buffer | D2 | §1 | wf §3 | `src/Utils/MpmcRingBuffer.hpp` | exists |
| Singleton helper | — | §6 | wf §5 | `src/Utils/Singleton.h` | exists |
| `UsbGate` (TX drain + RX dispatch) | D2 | §4, §5 | wf §3, wf §4 | `src/Core/UsbGate.h/.cpp` *(planned)* | Phase 3 |
| Transport abstraction / loopback | — | §5 | wf §4 | `src/Core/Transport.h` *(planned)* | Phase 3 |
| `Channel` + `runProcedure()` | D3, D6 | §1, §4 | wf §5 | `src/Core/Channel.h/.cpp` *(planned)* | Phase 4 |
| Workflow 1 — fire-and-forget | D6 | §4 | wf §6 | `src/Core/Channel.h` *(planned)* | Phase 4 |
| Workflow 2 — blocking send-and-wait | D6 | §4, §7 | wf §6 | `src/Core/Channel.h` *(planned)* | Phase 4 |
| Workflow 3 — non-blocking send-and-wait | D6 | §7 | wf §6 | `src/Core/Channel.h` *(planned)* | Phase 4 |
| `InstanceData` (`array<Channel,16>`) | — | §6 | wf §5 | `src/Core/InstanceData.h/.cpp` *(planned)* | Phase 5 |
| Shutdown ordering | — | §6 | wf §7 | `src/Core/InstanceData.cpp` *(planned)* | Phase 5 |
| pybind11 bindings | — | §5 | wf §8 | `bindings/` *(planned)* | Phase 6 |
| Build system | — | — | — | `CMakeLists.txt`, `GeneralTester.pro` | Phase 0 |
| Doc consistency check | — | — | — | `tools/check_docs.py` *(planned)* | Phase 1 |
| Legacy `DEI1016` (harvest source) | D2, D5 | §5 | — | `src/Hardware/DEI1016.h/.cpp` | exists (legacy) |
| Legacy ARINC 429 data model | — | — | — | `src/Items/Arinc429/` | exists (legacy) |
| Legacy Qt UI | — | — | — | `src/Widgets/` | exists (legacy) |
| Legacy event handlers (superseded) | — | — | — | `src/Workers/` | Phase 7 removes |

## Where each question is answered

| The question | Read |
|---|---|
| I am looking at this code — what is its story? | this file |
| What is being built, and in what order? | `PLAN.md` (repo root) |
| Why was this chosen? | `DECISIONS.md` |
| What rule generalises out of it? | `PRINCIPLES.md` |
| How does it run — which thread, which buffer? | `WORKFLOW.md` |
| What changed, and when? | `CHANGELOG.md` |
| What is built, what is next? | `PROGRESS.md` |
| How is work done here? | `CLAUDE.md` (repo root) |
| What was specified? | `Instructions.md` (workspace root) |

## Keeping this honest

A new principle, decision or mechanism section adds a row here **in the same change**.
Never move an argument into this file — the moment a row grows a paragraph, this stops
being a map.

When a *(planned)* path becomes real, the row is updated in the same commit that creates
it, and the "Created by" column keeps naming the phase. A row whose code column is stale
is worse than a missing row, because it is trusted.
