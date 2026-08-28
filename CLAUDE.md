# Working in this repository

OME++ is a multi-symbol order matching engine with a SQLite-backed account ledger.
`README.md` explains what it does. This file is about **how work is done here**, and
it exists because several of these conventions are load-bearing and none of them are
guessable from the code.

The short version: **documentation is part of the deliverable, not a follow-up.** A
change is finished when it is written into the project's own record, not when it
compiles.

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

---

## 2. Every change updates the documentation set

Four documents carry different things, and they are not interchangeable.

### `docs/markdowns/OME++_map.md` — the reverse index

Coordinates only, no arguments: one row per subsystem giving its theory section, its
decision, its principle, its workflow section and the code it landed in. **It is the
file to open first**, and the only one that answers "I am looking at this code, what
is its story" rather than requiring you to already know which principle to look up.

A new principle, decision or workflow section **adds a row here in the same change**.
And never move an argument into it — the moment a row grows a paragraph it becomes a
fifth document to keep in sync and stops being a map.

### `docs/markdowns/OME++_changelog.md` — what changed, and what it cost or fixed

The sections have distinct meanings and must be respected:

| § | Holds |
|---|---|
| 1 | Severe defects — money, integrity, or process survival |
| 2 | Improvements, one numbered subsection per phase or theme |
| 3 | Documentation corrections |
| 4 | Hypotheses that were wrong |
| 5 | Known limitations — **deliberate scope, explicitly not defects** |
| 6 | Known open defects |

A confirmed-but-unfixed bug goes in §6, never §5.

### `docs/markdowns/OME++_workflow.md` — how the mechanism actually works

Flow diagrams carry the document's own `[T: thread-name]` markers. Update the
**neighbouring** sections a change touches, not only the new one: a change to the
audit-record shape belongs in the audit-writer section as well as its own, and a new
cross-thread mechanism belongs in the concurrency-picture section too.

### `docs/markdowns/OME++_design_principles.md` — the reasoning that outlives both

**This document is a link in the decision chain, not a citation target.** The chain
runs principles → decision → code, and it runs in both directions: existing
principles constrain what gets built, and what gets built *extends* the principles
for the next change.

So when a change establishes a new invariant, or resolves a design tension in a way
future work should inherit, that reasoning belongs here as a numbered principle **in
the same change that introduces it**. A change that only cites principles has
consumed the chain without extending it. Add the summary-map row at the bottom too.

### `docs/markdowns/OME++_delivery_progress.md` — the live state of the plan

One entry per slice, newest last, each naming its commit. **A phase moves to done
only when its exit criterion passes as written** — not when its code exists. Read
this file first when picking work up again, and update it in the same commit as the
work.

### And it is checked, not merely asked for

```
python3 tools/check_docs.py       # or: ctest -R docs_consistency
```

Six checks, all of them mechanically falsifiable rather than matters of taste:
every `§` cross-reference resolves to a heading that exists, every principle has a
summary-map row, every file path named in the docs is real, the exported-metric
count quoted in any document matches what `src/Metrics.cpp` actually renders, every
`ome_*` metric named in prose is exported, and every principle has a row in
`OME++_map.md`.

It found 23 genuine dangling references on its first run — twenty of them left by a
file rename that had gone unnoticed for months. **Run it before committing a
documentation change**; a reference that no longer resolves is the one failure this
whole documentation set cannot survive quietly.

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

## 3. Plans are never rewritten to match reality

`OME++_plan.md` phase text is **not edited to match what happened**. Deviations get a
new "what X turned out to be" section instead, and struck-through text is preferred
over deletion.

A plan quietly rewritten to match reality is a worse record than one that says where
it missed. Follow this rather than tidying.

The same honesty applies to exit criteria: if a clause reads "detected, rebuilt,
alerted and metered, **with recovery time recorded**", then recording four of those
five is `PARTIAL`, not `PASS`. A delivery record that overstates a clause is worse
than one that omits it, because the omission gets noticed.

---

## 4. Verification protocol

Run all three before committing, not at the end of a phase.

```
cmake -S . -B build && cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

Then the sanitizers, which are separate build trees:

```
cmake -S . -B build-asan -DOME_SANITIZE=address,undefined -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build-tsan -DOME_SANITIZE=thread          -DCMAKE_BUILD_TYPE=Debug
```

Anything touching a thread, a lock or shared mutable state must be clean under
**ThreadSanitizer with no first-party suppression**. `tests/tsan_grpc.supp` covers the
distro gRPC binary only.

**`test_replay` fails, and that is expected**: its fixtures are generated data not
committed to the repository. Every other suite should be green. If something else
fails, it is yours.

One test binary per suite (`tests/CMakeLists.txt`), so a crash takes down only its
own suite and TSan can be pointed at just the concurrency suites.

---

## 5. Cost claims are measured, never asserted

`bench/` holds one benchmark per claim, and a phase that adds work to a hot path
carries a gate. Two rules learned the hard way:

- **Report the spread, not just the number.** A benchmark reporting single runs once
  had every mode of a feature coming out *faster* than the baseline it is a strict
  superset of. That is not a result; it is a harness whose noise floor exceeds its
  effect. Print a median and the observed spread beside it, so "no measurable cost"
  stays distinguishable from "we did not look hard enough".
- **A gate can be overtaken.** If building the thing a gate measures would itself
  violate the property the gate was protecting, say so, satisfy the property, and
  measure the part that genuinely lands on the hot path — then record why the reading
  differs from the one the plan asked for. See design principle §28.

---

## 6. House style in the code

The comments here are unusually dense and that is deliberate. They explain **why**,
and specifically what the alternative was and why it was rejected. Match that.

Recurring rules worth knowing before you write anything:

- **Money is fixed-point, never a double.** `Price` and `Quantity` are `int64_t` at
  `PRICE_SCALE` / `QTY_SCALE`. This holds for values arriving from outside too —
  parse venue decimals with `fixed_from_decimal_string`, not `std::stod`.
- **Absence is representable and never becomes zero.** `MarkPriceCache::mark` takes a
  `max_age` and there is deliberately no overload that drops it. A value that cannot
  be determined is `nullopt` all the way to the wire, where JSON carries `null` and
  protobuf carries an explicit `*_known` bool.
- **Single-writer ownership.** A `SymbolEngine` owns its book and matcher exclusively
  and reads them on the matching path with no lock. Anything changing that state from
  another thread goes **through the event queue**, never by writing the member.
- **The matching path holds no ledger.** `MatchingEngine` has a book and an order and
  nothing else. Work with ledger consequences is *reported out* and settled by the
  caller.
- **Refuse rather than default.** An unrecognised enum name, a zero interval, a
  missing credential — refuse it where it is read and say so. A misspelling that
  silently becomes the default leaves the venue running a policy nobody chose.
- **Name what you could not do.** Truncation, unpriced assets, unverified symbols and
  dropped rows are all reported explicitly, because "empty" and "we stopped looking"
  are opposite claims.

---

## 7. Layout

| Path | Holds |
|---|---|
| `include/`, `src/` | The engine. One venue client per file; `ome_lib` is everything except `main`. |
| `sql/` | Schemas, embedded at configure time into `EmbeddedSchema.hpp` — edit the `.sql`, never the generated header, and add the `file(READ …)` line in `CMakeLists.txt`. |
| `tests/` | One binary per suite. `crash_injection.sh` is a driver, deliberately not a ctest target. |
| `bench/` | One benchmark per cost claim. |
| `tools/` | `backup.sh` / `restore.sh`. The restore *proves* itself by running `--check-and-exit`. |
| `docs/markdowns/` | The record. See §2. |
| `deploy/` | Dockerfile. **Never built here** — no container runtime on the build machine. |

Build options are exclusive where they must be: `OME_INTAKE_RPC` is `jsonrpc`,
`grpc` or `none`, and an invalid combination is a configure-time fatal error rather
than a runtime surprise.
