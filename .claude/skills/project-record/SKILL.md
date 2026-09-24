---
name: project-record
description: Set up and maintain a written record for exploratory, multi-session work — where the destination is not known at the start and the thinking has to survive between sessions. Creates a five-artifact document set, the conventions that keep it honest, and a script that checks its cross-references mechanically. Use at the start of a substantial project, or when an existing one has outgrown its README.
---

# The project record

## What this is for, and when NOT to use it

This is for work that is **exploratory and long-running**: you do not yet know
where it ends, it will span many sessions, and the reasoning will not survive in
anyone's head between them.

It is **not** for a well-specified task, a short project, or anything you could
finish in a sitting. The overhead is real and only pays back across sessions.

The central idea, and the thing that makes the shape make sense:

> **These documents are not a record of the thinking. They are where the thinking
> happens.** A decision is made *by writing it down* — the writing is what forces
> the alternative to be named and rejected. Nobody designs this as a protocol; it
> becomes one by being used, and its purpose usually becomes clear only in
> hindsight.

That is why the conventions below look like pure overhead to anyone who assumes
the destination is known at the start. They are not overhead. They are what makes
the work **resumable**.

---

## The five artifacts

Each answers a different question, in a different tense. They are not
interchangeable and none is a summary of another.

| Artifact | Question it answers | Tense |
|---|---|---|
| `MAP.md` | *I am looking at this code — what is its story?* | now |
| `DECISIONS.md` | *Why was this chosen, and what did it cost?* | when decided |
| `PRINCIPLES.md` | *What rule generalises out of it?* | forward |
| `PROGRESS.md` | *What is built, what is next?* | live |
| `CHANGELOG.md` | *What changed, and what broke or was fixed?* | history |

Two more, added **only when they earn it**:

- **`THEORY.md`** — why this is the right shape at all, beneath the level of any
  decision. Add it when the project rests on something non-obvious (a
  mathematical model, a protocol, a physical constraint). Most projects do not
  need it. It is load-bearing exactly when a decision *cites* it and decorative
  otherwise.
- **`WORKFLOW.md`** — how the running system actually behaves: what calls what,
  on which thread, in what order. This is the one genuine piece of *reference*
  documentation in the set, and the only one a reader might read start to finish.

**`MAP.md` is the entry point and the most valuable of the five.** Every other
document is a forward index organised on its own axis, traversable only if you
already know which entry to start from. The map is the reverse index — the one
you can enter with a file open in front of you. Build it early; it is the piece
most often missing.

---

## The conventions that keep it honest

These are the load-bearing part. The documents without them rot within weeks.

**1. Decisions are numbered and permanent.**
`D1`, `D2`, … Once assigned, a number never changes meaning. A later decision
*cites* an earlier one rather than relitigating it. Each entry names: the choice,
the alternative rejected, the cost accepted, and what bears on it.

**2. Plans are never rewritten to match reality.**
When something turns out differently, **add** a "what it turned out to be"
section; leave the original text standing. Prefer struck-through text to
deletion. *A plan quietly corrected is a worse record than one that says where it
missed* — the gap between them is the most valuable thing in the document.

**3. There is a section for hypotheses that were wrong.**
The discarded explanation is usually the expensive thing to rediscover, and it is
the first thing a tidy-minded editor deletes. Give it a heading so it cannot be
mistaken for clutter.

**4. Exit criteria are read literally.**
If a criterion says "detected, rebuilt, alerted and metered, **with recovery time
recorded**", then four of five is `PARTIAL`, not `PASS`. **A record that
overstates is worse than one that omits**, because the omission gets noticed.

**5. Every change updates the record in the same commit.**
Not as a follow-up. A change is finished when it is written down, not when it
compiles. This is the convention that fails first if it is ever treated as
optional.

**6. The principles document is a link in the chain, not a citation target.**
The chain runs *principles → decision → code*, and it runs both ways: existing
principles constrain what gets built, and what gets built **extends** them. A
change that only cites a principle has consumed the chain without extending it.
When a change establishes a new invariant, add it as a numbered principle in the
same commit, with a row in the summary map and in `MAP.md`.

**7. One argument, one home.**
State a piece of reasoning in full in exactly one document; everywhere else
points at it. Code comments are the exception — the header of the class that
implements something keeps its reasoning, because its editor needs it there.
Duplication is not a style problem: it is two things that will silently disagree.

**8. Cost and performance claims are measured, and report their own spread.**
A benchmark that reports a single run will eventually show a strict superset of
work running *faster* than its baseline. That is not a result; it is a harness
whose noise floor exceeds its effect. Print a median and the observed spread, so
"no measurable difference" stays distinguishable from "we did not look hard
enough".

---

## Bootstrapping a project

1. **Create `MAP.md` first**, even nearly empty. It is the file everything else
   gets indexed into, and adding it late means back-filling.
2. Create `DECISIONS.md`, `PRINCIPLES.md`, `PROGRESS.md`, `CHANGELOG.md` from the
   templates below.
3. Write the conventions into `CLAUDE.md` at the repo root, so they bind every
   future session and every collaborator rather than living in one person's head.
4. Add the consistency check (below) and wire it into the test target.
5. **Put a "Start here" block at the top of `README.md`**: current state in about
   six lines, then a *which door do I want* table. If the big picture requires
   opening three documents, there is no entry point.

### The kickoff prompt

Paste at the start of a project, and adjust the first two lines:

> I'm starting work on **\<what it is\>**. It will run over many sessions and I
> don't yet know exactly where it's going — expect the shape to change.
>
> Use the `project-record` skill. Set up the document set and the conventions
> before we write code, and from then on treat updating the record as part of
> every change rather than something I have to ask for.
>
> Two things I want you to do without being asked: **tell me when you think I'm
> framing something wrong**, and **when a change establishes a rule future work
> should follow, add it to `PRINCIPLES.md` in that same change.**
>
> If at any point you can't tell what a piece of this is *for*, ask me — I may
> not know either, and saying so is useful.

---

## Templates

### `MAP.md`

```markdown
# The Map — open a file, find its story

This document contains no arguments, only coordinates. Every other document is a
forward index on its own axis; this is the reverse one.

| Subsystem | Theory | Decision | Principle | Mechanism | Code |
|---|---|---|---|---|---|
| ... | §n | Dn | §n | wf §n | `path` |

## Where each question is answered
| The question | Read |
|---|---|
| I am looking at this code — what is its story? | this file |
| Why was this chosen? | `DECISIONS.md` |
| What rule generalises out of it? | `PRINCIPLES.md` |
| How does it run? | `WORKFLOW.md` |
| What changed, and when? | `CHANGELOG.md` |
| What is built, what is next? | `PROGRESS.md` |
| How is work done here? | `CLAUDE.md` |

## Keeping this honest
A new principle, decision or mechanism section adds a row here **in the same
change**. Never move an argument into this file — the moment a row grows a
paragraph, this stops being a map.
```

### `DECISIONS.md`

```markdown
# Decisions

Numbered, permanent, never renumbered. A later decision cites an earlier one
rather than reopening it.

| | Decision | Full text | Bears on it |
|---|---|---|---|
| **D1** | one line | §1 | theory §n / principle §n |

## D1 — <the choice, as a claim>
**The decision:** what was chosen.
**Rejected:** the alternative, and why it lost. *This is the part with value.*
**Cost:** what was accepted in exchange. Measured where measurable.
**Revisit if:** the condition that would reopen this.
```

### `PROGRESS.md`

```markdown
# Progress — the live state

One entry per slice, newest last, each naming its commit. A phase is **done**
only when its exit criterion passes **as written**.

| Phase | State | Exit criterion status |
|---|---|---|

### Slice N — <what it built> · <state>
**Built:** …
**What surprised us:** the finding. Often the most valuable line here.
**Exit clauses:** which moved, and which did not — `PARTIAL` is not `PASS`.
**Verified:** tests, sanitizers, commit.
```

### `CHANGELOG.md`

Six sections, and the distinctions between them are the point:

```markdown
## 1. Severe defects        — correctness, data, or process survival
## 2. Improvements          — one subsection per theme
## 3. Documentation corrections
## 4. Hypotheses that were wrong    <- do not delete this section
## 5. Known limitations     — DELIBERATE scope, explicitly not defects
## 6. Known defects, open   — confirmed and not yet fixed
```

A confirmed-but-unfixed bug belongs in §6, never §5. The two invite opposite
responses.

---

## The consistency check

The whole set is held together by cross-references, and its one weakness is that
**it works because people are careful**. A renamed section leaves a reference
pointing nowhere and *nothing fails*.

Write a script — `tools/check_docs.py` in this repository is a worked example —
that verifies only mechanically falsifiable claims, so every failure is
actionable and never a matter of taste:

1. Every `§` cross-reference resolves to a heading that exists
2. Every principle has a row in the summary map **and** in `MAP.md`
3. Every file path named in the docs exists
4. Any number the docs quote that can be recomputed from source, is

Wire it into the test target so it runs with everything else.

**Two things that matter more than the checks.** Accept heading formats
generously: a checker that reports false failures gets switched off, and then it
also consumes the attention that would have caught the problem by hand. And run
it early — the version in this repository found **23 real dangling references on
its first run**, twenty of them from a file rename months earlier that the
document set gave no sign of.

---

## What this cannot do

It cannot tell you whether the prose is *right*. Nothing can. What it does is
make the record cheap to keep, expensive to falsify by accident, and possible to
resume — which is the whole of the problem when the destination is not known at
the start.
