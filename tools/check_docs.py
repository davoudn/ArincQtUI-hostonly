#!/usr/bin/env python3
"""Mechanically check the documentation set's cross-references.

Every check here is falsifiable from the files themselves — no check depends on
whether the prose is *right*, only on whether it is *consistent*. A checker that
reports false failures gets switched off, and then it also consumes the attention
that would have caught a real problem by hand. So the checks are deliberately
narrow, and heading formats are accepted generously.

Checks:
  1. every ``§n`` reference in a document resolves to a heading that exists
     *in that document or in the document named alongside it*
  2. every principle has a row in PRINCIPLES.md's summary map
  3. every decision Dn referenced anywhere exists in DECISIONS.md
  4. every relative file path named in backticks in the docs exists on disk,
     unless it is marked *(planned)*
  5. every doc in docs/markdowns/ is reachable from MAP.md
  6. no leftover merge-conflict markers anywhere in the repo's tracked docs

Exit status is 0 when clean, 1 when anything failed. Run before committing a
documentation change; wire into ctest once the build configures (PLAN.md Phase 0).
"""

import os
import re
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOCS_DIR = os.path.join(REPO_ROOT, "docs", "markdowns")

# The doc set. MAP.md is the entry point and the reverse index.
DOCS = [
    "MAP.md",
    "DECISIONS.md",
    "PRINCIPLES.md",
    "WORKFLOW.md",
    "CHANGELOG.md",
    "PROGRESS.md",
]
ROOT_DOCS = ["CLAUDE.md", "PLAN.md", "README"]

# Short names used in prose for the docs, so "wf §3" and "WORKFLOW §3" both resolve.
DOC_ALIASES = {
    "wf": "WORKFLOW.md",
    "workflow": "WORKFLOW.md",
    "decisions": "DECISIONS.md",
    "principles": "PRINCIPLES.md",
    "principle": "PRINCIPLES.md",
    "changelog": "CHANGELOG.md",
    "progress": "PROGRESS.md",
    "map": "MAP.md",
    "plan": "PLAN.md",
}


def read(path):
    with open(path, encoding="utf-8") as f:
        return f.read()


def headings(text):
    """Section numbers claimed by a document's own headings.

    Accepts '## §1 —', '## 1.', '## D1 —', '### 6.2 —', '## §1'. Generous on
    purpose: the point is to catch a reference to a section that was deleted or
    renumbered, not to police heading punctuation.
    """
    found = set()
    for line in text.splitlines():
        if not line.lstrip().startswith("#"):
            continue
        h = line.lstrip("#").strip()
        for m in re.finditer(r"§\s*(\d+)", h):
            found.add(m.group(1))
        for m in re.finditer(r"^(\d+)[.\s—-]", h):
            found.add(m.group(1))
        for m in re.finditer(r"\bD(\d+)\b", h):
            found.add("D" + m.group(1))
        # subsection numbers like 6.2, 2.1, 4.1
        for m in re.finditer(r"^(\d+\.\d+)\b", h):
            found.add(m.group(1))
    return found


def check_section_refs(failures):
    """Check 1: §n references resolve, in the doc named alongside them."""
    all_sections = {}
    for name in DOCS + ROOT_DOCS:
        path = os.path.join(DOCS_DIR if name in DOCS else REPO_ROOT, name)
        if os.path.exists(path):
            all_sections[name] = headings(read(path))

    ref_re = re.compile(
        r"(?:(`?(?:docs/markdowns/)?([A-Z_]+\.md)`?)|(\bwf\b|\bprinciple\b|\bPLAN\b))"
        r"[^\n§]{0,40}?§\s*(\d+(?:\.\d+)?)"
    )
    for name, path in _doc_paths():
        text = read(path)
        for lineno, line in enumerate(text.splitlines(), 1):
            for m in ref_re.finditer(line):
                target = m.group(2) or DOC_ALIASES.get((m.group(3) or "").lower(), name)
                sec = m.group(4)
                if target not in all_sections:
                    continue  # named a doc outside the set; not our business
                if sec not in all_sections[target]:
                    failures.append(
                        f"{name}:{lineno}: §{sec} not found in {target}"
                    )

    # bare '§n' with no doc named -> resolves against the same document
    bare_re = re.compile(r"(?<![.\w])§\s*(\d+(?:\.\d+)?)")
    for name, path in _doc_paths():
        text = read(path)
        for lineno, line in enumerate(text.splitlines(), 1):
            if ref_re.search(line):
                continue  # already handled above with an explicit target
            for m in bare_re.finditer(line):
                sec = m.group(1)
                if sec not in all_sections.get(name, set()):
                    failures.append(f"{name}:{lineno}: §{sec} not found in {name} itself")


def _doc_paths():
    for name in DOCS:
        yield name, os.path.join(DOCS_DIR, name)
    for name in ROOT_DOCS:
        p = os.path.join(REPO_ROOT, name)
        if os.path.exists(p):
            yield name, p


def check_principle_summary_rows(failures):
    """Check 2: every principle §n has a row in the summary map."""
    path = os.path.join(DOCS_DIR, "PRINCIPLES.md")
    if not os.path.exists(path):
        return
    text = read(path)
    defined = set(re.findall(r"^##\s*§\s*(\d+)", text, re.M))
    summary = text.split("## Summary map")[-1]
    mapped = set(re.findall(r"^\|\s*(\d+)\s*\|", summary, re.M))
    for n in sorted(defined - mapped, key=int):
        failures.append(f"PRINCIPLES.md: principle §{n} has no summary-map row")
    for n in sorted(mapped - defined, key=int):
        failures.append(f"PRINCIPLES.md: summary-map row §{n} has no principle section")


def check_decision_refs(failures):
    """Check 3: every Dn cited anywhere is defined in DECISIONS.md."""
    dpath = os.path.join(DOCS_DIR, "DECISIONS.md")
    if not os.path.exists(dpath):
        return
    defined = set(re.findall(r"^##\s*(D\d+)\b", read(dpath), re.M))
    cited = re.compile(r"\bD(\d{1,2})\b")
    for name, path in _doc_paths():
        for lineno, line in enumerate(read(path).splitlines(), 1):
            for m in cited.finditer(line):
                d = "D" + m.group(1)
                # skip things that are not decisions: 'D4's' is, '2D' is not,
                # and a table of hex/ids would false-positive, so require a
                # word boundary before D and not a digit after.
                if d not in defined:
                    failures.append(f"{name}:{lineno}: {d} cited but not defined in DECISIONS.md")


def check_paths(failures):
    """Check 4: backticked relative paths exist, unless marked *(planned)*."""
    path_re = re.compile(r"`((?:src|docs|tools|bindings|Tests|Icons|CustomIcons|bench)/[^`\s]+)`")
    for name, path in _doc_paths():
        text = read(path)
        for lineno, line in enumerate(text.splitlines(), 1):
            if "(planned)" in line or "*does not exist yet*" in line.lower():
                continue
            for m in path_re.finditer(line):
                rel = m.group(1).rstrip(".,;:")
                # a glob-ish or header/cpp pair reference is not a single file
                if any(c in rel for c in "*"):
                    continue
                full = os.path.join(REPO_ROOT, rel)
                if not os.path.exists(full):
                    # allow 'Foo.h/.cpp' style by testing the first suffix
                    if not os.path.exists(os.path.join(REPO_ROOT, rel.split("/")[0])):
                        failures.append(f"{name}:{lineno}: path does not exist: {rel}")
                        continue
                    failures.append(f"{name}:{lineno}: path does not exist: {rel}")


def check_map_reachability(failures):
    """Check 5: every doc in the set is named somewhere in MAP.md."""
    mpath = os.path.join(DOCS_DIR, "MAP.md")
    if not os.path.exists(mpath):
        failures.append("MAP.md is missing — it is the entry point and must exist")
        return
    text = read(mpath)
    for name in DOCS + ROOT_DOCS:
        if name == "MAP.md":
            continue
        if name not in text:
            failures.append(f"MAP.md does not reference {name}")
def check_conflict_markers(failures):
    """Check 6: no merge-conflict markers in the documentation set."""
    marker = re.compile(r"^(<<<<<<<|>>>>>>>|=======)$")
    for name, path in _doc_paths():
        for lineno, line in enumerate(read(path).splitlines(), 1):
            if marker.match(line.strip()):
                failures.append(f"{name}:{lineno}: leftover conflict marker")


def main():
    failures = []
    check_section_refs(failures)
    check_principle_summary_rows(failures)
    check_decision_refs(failures)
    check_paths(failures)
    check_map_reachability(failures)
    check_conflict_markers(failures)

    if failures:
        print(f"check_docs: {len(failures)} failure(s)")
        for f in failures:
            print(f"  {f}")
        return 1
    print("check_docs: clean")
    return 0


if __name__ == "__main__":
    sys.exit(main())
