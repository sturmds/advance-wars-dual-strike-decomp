#!/usr/bin/env python3
"""Record that the tenets review ran on a change; list commits without one.

    review_stamp.py                 stamp the staged change (or HEAD if nothing is staged)
    review_stamp.py REV...          stamp these commits
    review_stamp.py --missing [UPSTREAM]
                                    list commits in UPSTREAM..HEAD (default mainline)
                                    that need a stamp and have none; exit 1 if any

The last step of the tenets review (.claude/skills/tenets-review/SKILL.md)
runs this. land_self.py refuses to land a branch with commits that need a
stamp and have none (exit 5).

A stamp is a file <git common dir>/tenets-review/<patch-id> that holds
the time and what was stamped.
The patch ID (`git patch-id --stable`) depends only on the diff, so a
stamp taken on the staged change before `git commit` fits the commit, and
it still fits after the commit is replayed onto a newer mainline without
changes. PROGRESS.md is left out of the diff: land_self.py amends it into
the last commit. Stamps live under the common git directory, so they never
enter git and every worktree sees them. When rebase_onto_mainline.py
replays a stamped commit and the merge changes its diff, the new commit is
stamped too.

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
PATHSPEC = ["--", ".", ":(exclude)PROGRESS.md"]


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


def is_stamped(rev: str) -> bool:
    pid = commit_patch_id(rev)
    return pid is not None and (stamp_dir() / pid).exists()


def write_stamp(pid: str, what: str) -> None:
    d = stamp_dir()
    d.mkdir(exist_ok=True)
    when = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    (d / pid).write_text(f"{when} {what}\n")


def carry(old: str, new: str) -> None:
    """A replay turned commit `old` into `new`: if `old` was reviewed, so is `new`."""
    if is_stamped(old):
        pid = commit_patch_id(new)
        if pid and not (stamp_dir() / pid).exists():
            write_stamp(pid, f"replay of {old[:10]} as {new[:10]}")


def needs_stamp(rev: str) -> bool:
    lines = git("diff-tree", "--root", "--no-commit-id", "--no-renames", "-r", "--name-status", rev).splitlines()
    changes = [line.split("\t", 1) for line in lines if "\t" in line]
    code = [(status, path) for status, path in changes if path.startswith(("src/", "include/"))]
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


def main() -> int:
    ap = argparse.ArgumentParser(description="Record that the tenets review ran on a change, or list"
                                 " commits that need a review stamp and have none.")
    ap.add_argument("revs", nargs="*", metavar="REV",
                    help="commits to stamp (default: the staged change, or HEAD if nothing is staged)")
    ap.add_argument("--missing", nargs="?", const="mainline", metavar="UPSTREAM",
                    help="list commits in UPSTREAM..HEAD (default mainline) that need a stamp and"
                    " have none; exit 1 if any")
    args = ap.parse_args()
    if args.missing is not None:
        if args.revs:
            ap.error("--missing takes no REV")
        todo = missing(args.missing)
        for c in todo:
            print(describe(c))
        return 1 if todo else 0
    if args.revs:
        for rev in args.revs:
            pid = commit_patch_id(rev)
            if pid is None:
                print(f"{describe(rev)}: no changes to stamp")
                continue
            write_stamp(pid, describe(rev))
            print(f"stamped {describe(rev)}")
        return 0
    pid = staged_patch_id()
    if pid is not None:
        write_stamp(pid, "staged change")
        print("stamped the staged change; commit it as it is (any later edit needs a new review and stamp)")
        return 0
    pid = commit_patch_id("HEAD")
    if pid is None:
        sys.exit("error: nothing staged and HEAD has no changes")
    write_stamp(pid, describe("HEAD"))
    print(f"nothing staged; stamped {describe('HEAD')}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
