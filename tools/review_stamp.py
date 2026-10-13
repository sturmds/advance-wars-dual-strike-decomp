#!/usr/bin/env python3
"""Record that the tenets review ran on a change; list commits without one.

    review_stamp.py --findings FILE          stamp the staged change (or HEAD if nothing is staged)
    review_stamp.py --findings FILE REV...   stamp these commits
    review_stamp.py --missing [UPSTREAM]     list commits in UPSTREAM..HEAD (default mainline)
                                             that need a stamp and have none; exit 1 if any
    review_stamp.py --show REV               print the findings stamped for REV

The last step of the tenets review (.claude/skills/tenets-review/SKILL.md)
runs this. land_self.py refuses to land a branch with commits that need a
stamp and have none (exit 5), and prints the findings of the others.

FILE holds what the review checked and found: for each file the change
adds or edits under src/ or include/, at least one line
`<path>: <what was checked and found>` with 20 characters or more after
the colon. The stamp is refused when a file has no such line, or when
tools/style_check.py finds a banned word in one of those files, so the
mechanical part of the review always runs.

A stamp is a file <git common dir>/tenets-review/<patch-id> that holds
the time, what was stamped and the findings. Stamps without findings
(written before findings were required) count as missing.
The patch ID (`git patch-id --stable`) depends only on the diff, so a
stamp taken on the staged change before `git commit` fits the commit, and
it still fits after the commit is replayed onto a newer mainline without
changes. PROGRESS.md and PROGRESS.json are left out of the diff:
land_self.py amends them into the last commit. Stamps live under the common
git directory, so they never enter git and every worktree sees them. When rebase_onto_mainline.py
replays a stamped commit with no stop (git's merge and header_merge.py's
field renames change the diff, nobody else does), the new commit gets the
old stamp. A commit fixed by hand during `--continue` does not: it needs a
new review.

A commit needs a stamp when it touches src/ or include/, except:
- `Propagate ...` commits that only add files under src/ (and config/):
  propagate.py wrote the C from a reviewed template, and twins keep their
  address names.
- `WIP ...` commits that do not touch include/: the file stays unlinked,
  and the commit that finishes it is reviewed. A WIP commit that changes a
  shared header needs a stamp.
"""

import argparse
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

DIFF_OPTS = ["-p", "--no-renames", "--no-color", "--no-ext-diff", "--full-index"]
PATHSPEC = ["--", ".", ":(exclude)PROGRESS.md", ":(exclude)PROGRESS.json"]
CODE = ("src/", "include/")
MIN_NOTE = 20  # characters after `<path>:`, so "ok" does not pass
FINDINGS = "findings:\n"  # separates the stamp's first line from the findings


def git(*args: str, stdin: str | None = None) -> str:
    r = subprocess.run(["git", *args], input=stdin, capture_output=True, text=True)
    if r.returncode:
        sys.exit(f"error: git {' '.join(args)}: {r.stderr.strip()}")
    return r.stdout


def stamp_dir() -> Path:
    return Path(git("rev-parse", "--path-format=absolute", "--git-common-dir").strip()) / "tenets-review"


def patch_id_of(diff: str) -> str | None:
    if not diff.strip():
        return None
    out = git("patch-id", "--stable", stdin=diff).split()
    return out[0] if out else None


def commit_patch_id(rev: str) -> str | None:
    return patch_id_of(git("diff-tree", "--root", *DIFF_OPTS, rev, *PATHSPEC))


def staged_patch_id() -> str | None:
    return patch_id_of(git("diff", "--cached", *DIFF_OPTS, *PATHSPEC))


def findings_of(rev: str) -> str:
    """The findings stamped for `rev`'s diff, or "" if it has none."""
    pid = commit_patch_id(rev)
    path = stamp_dir() / pid if pid else None
    if path is None or not path.exists():
        return ""
    text = path.read_text()
    return text.split(FINDINGS, 1)[1] if FINDINGS in text else ""


def is_stamped(rev: str) -> bool:
    return bool(findings_of(rev).strip())


def write_stamp(pid: str, what: str, findings: str) -> None:
    d = stamp_dir()
    d.mkdir(exist_ok=True)
    when = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    (d / pid).write_text(f"{when} {what}\n{FINDINGS}{findings}")


def carry(old: str, new: str) -> None:
    """A replay with no hand edits turned commit `old` into `new`: if `old`
    was reviewed, so is `new`. Not called for a commit fixed during --continue."""
    findings = findings_of(old)
    pid = commit_patch_id(new)
    if findings and pid and not (stamp_dir() / pid).exists():
        write_stamp(pid, f"replay of {old[:10]} as {new[:10]}", findings)


def code_files(*diff_args: str) -> list[str]:
    """Files under src/ or include/ that the diff adds or edits."""
    lines = git(*diff_args, "--no-renames", "--name-only", "--diff-filter=d").splitlines()
    return [path for path in lines if path.startswith(CODE)]


def uncovered(findings: str, files: list[str]) -> list[str]:
    """The files that have no findings line of MIN_NOTE characters."""
    longest: dict[str, int] = {}
    for line in findings.splitlines():
        path, colon, note = line.partition(":")
        if colon:
            longest[path.strip()] = max(longest.get(path.strip(), 0), len(note.strip()))
    return [path for path in files if longest.get(path, 0) < MIN_NOTE]


def banned_words(texts: dict[str, str]) -> int:
    """Run style_check.py on the files' contents; the number of errors."""
    import style_check  # imported here, so land_self.py runs without it
    banned = style_check.load_banned()
    return sum(style_check.check(name, text, banned, [], False) for name, text in texts.items())


def stamp(pid: str, what: str, files: dict[str, str], findings_file: str) -> bool:
    """Write the stamp if the findings cover `files` (path -> content) and
    style_check.py passes them; otherwise print why and return False."""
    findings = Path(findings_file).read_text()
    missed = uncovered(findings, sorted(files))
    for path in missed:
        print(f"{findings_file}: no line `{path}: <what was checked and found>`"
              f" with {MIN_NOTE} or more characters after the colon", file=sys.stderr)
    if missed or banned_words(files):
        print(f"not stamped: {what}. Do the review in .claude/skills/tenets-review/SKILL.md"
              " and fix the above first.", file=sys.stderr)
        return False
    write_stamp(pid, what, findings)
    return True


def needs_stamp(rev: str) -> bool:
    lines = git("diff-tree", "--root", "--no-commit-id", "--no-renames", "-r", "--name-status", rev).splitlines()
    changes = [line.split("\t", 1) for line in lines if "\t" in line]
    code = [(status, path) for status, path in changes if path.startswith(CODE)]
    if not code:
        return False
    subject = git("log", "-1", "--format=%s", rev).strip()
    headers = any(path.startswith("include/") for _, path in code)
    if subject.startswith("Propagate ") and not headers and all(status == "A" for status, _ in code):
        return False
    if subject.startswith("WIP ") and not headers:
        return False
    return True


def branch_commits(upstream: str) -> list[str]:
    """The commits land_self.py would replay: merge base..HEAD, without
    those already on upstream under the same subject."""
    base = git("merge-base", "HEAD", upstream).strip()
    commits = git("rev-list", "--reverse", "--no-merges", f"{base}..HEAD").split()
    landed = set(git("log", "--format=%s", f"{base}..{upstream}").splitlines())
    return [c for c in commits if git("log", "-1", "--format=%s", c).strip() not in landed]


def missing(upstream: str = "mainline") -> list[str]:
    return [c for c in branch_commits(upstream) if needs_stamp(c) and not is_stamped(c)]


def describe(rev: str) -> str:
    return git("log", "-1", "--format=%h %s", rev).strip()


def stamp_revs(revs: list[str], findings_file: str) -> int:
    ok = True
    for rev in revs:
        pid = commit_patch_id(rev)
        if pid is None:
            print(f"{describe(rev)}: no changes to stamp")
            continue
        files = {f: git("show", f"{rev}:{f}") for f in code_files("diff-tree", "--root", "-r", rev)}
        if stamp(pid, describe(rev), files, findings_file):
            print(f"stamped {describe(rev)}")
        else:
            ok = False
    return 0 if ok else 1


def main() -> int:
    ap = argparse.ArgumentParser(description="Record that the tenets review ran on a change, or list"
                                 " commits that need a review stamp and have none.")
    ap.add_argument("revs", nargs="*", metavar="REV",
                    help="commits to stamp (default: the staged change, or HEAD if nothing is staged)")
    ap.add_argument("--findings", metavar="FILE",
                    help="what the review checked and found: `<path>: <findings>` lines, at least one"
                    " per file the change adds or edits under src/ or include/")
    ap.add_argument("--missing", nargs="?", const="mainline", metavar="UPSTREAM",
                    help="list commits in UPSTREAM..HEAD (default mainline) that need a stamp and"
                    " have none; exit 1 if any")
    ap.add_argument("--show", metavar="REV", help="print the findings stamped for REV")
    args = ap.parse_args()
    if args.missing is not None:
        if args.revs:
            ap.error("--missing takes no REV")
        todo = missing(args.missing)
        for c in todo:
            print(describe(c))
        return 1 if todo else 0
    if args.show:
        print(findings_of(args.show) or f"{describe(args.show)}: no stamp with findings\n", end="")
        return 0
    if not args.findings:
        ap.error("--findings FILE is required; the tenets review writes it"
                 " (.claude/skills/tenets-review/SKILL.md, step 4)")
    if args.revs:
        return stamp_revs(args.revs, args.findings)
    pid = staged_patch_id()
    if pid is None:
        print("nothing staged; stamping HEAD")
        return stamp_revs(["HEAD"], args.findings)
    files = {f: git("show", f":{f}") for f in code_files("diff", "--cached")}
    if not stamp(pid, "staged change", files, args.findings):
        return 1
    print("stamped the staged change; commit it as it is (any later edit needs a new review and stamp)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
