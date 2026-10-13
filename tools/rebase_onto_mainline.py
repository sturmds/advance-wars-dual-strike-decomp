#!/usr/bin/env python3
"""Replay this branch's own commits onto the current mainline.

    rebase_onto_mainline.py              start
    rebase_onto_mainline.py --onto REV   start, onto REV instead of mainline (for tests)
    rebase_onto_mainline.py --continue   after fixing a stop
    rebase_onto_mainline.py --abort      go back to the branch as it was

Run inside an agent's worktree before reporting. Other agents land on
mainline while this one works; replaying its commits here, where the
agent still knows its code, leaves the orchestrator a replay with nothing
to merge.

It takes the commits from the merge base with mainline to HEAD, replays
each with tools/apply_commit.py (so headers and config merge by meaning)
on a temporary branch started at mainline, then moves this branch there.
Each replayed commit runs the hooks, so `make release` must pass.

When a replay stops (a conflict, or a failed `make release`), the changes
are staged. Fix them, `git add` the files, and run it with --continue.
PROGRESS.md is never committed here: mainline writes it when the commit
lands.

A commit replayed without a stop keeps its tenets review stamp
(tools/review_stamp.py), also when the merge changed its diff. A commit
committed by --continue was fixed by hand, so it needs a new review and
stamp before land_self.py lands it.

It refuses to run while land_self.py or another rebase_onto_mainline.py
runs in this worktree: both hold <git dir>/land-self.lock.
"""

import argparse
import fcntl
import json
import os
import subprocess
import sys
from pathlib import Path

import review_stamp


def git(*args, check=True) -> str:
    r = subprocess.run(["git", *args], capture_output=True, text=True)
    if check and r.returncode:
        sys.exit(f"error: git {' '.join(args)}: {r.stderr.strip()}")
    return r.stdout.strip()


ROOT = Path(git("rev-parse", "--show-toplevel"))
STATE = Path(git("rev-parse", "--path-format=absolute", "--git-path", "rebase-onto-mainline.json"))
WORKTREE_LOCK = Path(git("rev-parse", "--path-format=absolute", "--git-path", "land-self.lock"))


def lock_worktree():
    """Take the worktree lock that land_self.py also holds, unless the
    land_self.py run that started this one holds it already."""
    if os.environ.get("LAND_SELF_LOCK_HELD") == "1":
        return None
    f = open(WORKTREE_LOCK, "a+")
    try:
        fcntl.flock(f, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        f.seek(0)
        holder = f.read().strip()
        sys.exit(f"error: another land_self.py or rebase_onto_mainline.py runs in this worktree"
                 f"{' (' + holder + ')' if holder else ''}. Wait for it with\n    flock {WORKTREE_LOCK} true")
    f.seek(0)
    f.truncate()
    f.write(f"pid {os.getpid()}, {' '.join(sys.argv)}\n")
    f.flush()
    return f


def save(state: dict) -> None:
    STATE.write_text(json.dumps(state, indent=1))


def start() -> int:
    if STATE.exists():
        sys.exit("error: a rebase is already under way; use --continue or --abort")
    branch = git("branch", "--show-current")
    if not branch or branch == "mainline":
        sys.exit("error: run this on the worktree's own branch, not on mainline or a detached HEAD")
    if git("status", "--porcelain", "--untracked-files=no"):
        sys.exit("error: the working tree has uncommitted changes; commit them first")
    onto = sys.argv[sys.argv.index("--onto") + 1] if "--onto" in sys.argv else "mainline"
    mainline = git("rev-parse", onto)
    base = git("merge-base", "HEAD", mainline)
    commits = git("rev-list", "--reverse", "--no-merges", f"{base}..HEAD").split()
    # A commit this branch shares with another agent's may already be on
    # mainline under a new hash (apply_commit replays it); match by subject.
    landed = set(git("log", "--format=%s", f"{base}..{mainline}").splitlines())
    for c in list(commits):
        subject = git("log", "-1", "--format=%s", c)
        if subject in landed:
            print(f"skipping {c[:10]} ({subject}): already on mainline")
            commits.remove(c)
    if base == mainline or not commits:
        print(f"{branch} already starts at mainline {mainline[:10]}; nothing to do")
        return 0
    head = git("rev-parse", "HEAD")
    temp = f"{branch}-rebase"
    git("switch", "-q", "-C", temp, mainline)
    state = {"branch": branch, "temp": temp, "orig": head, "todo": commits, "done": []}
    save(state)
    print(f"replaying {len(commits)} commit(s) of {branch} onto mainline {mainline[:10]}")
    return replay(state)


def replay(state: dict) -> int:
    while state["todo"]:
        commit = state["todo"][0]
        r = subprocess.run([sys.executable, str(Path(__file__).resolve().parent / "apply_commit.py"), commit], cwd=ROOT)
        if r.returncode:
            stop(commit)
            return 1
        done(state, commit, reviewed=True)
    return finish(state)


def done(state: dict, commit: str, reviewed: bool) -> None:
    new = git("rev-parse", "HEAD")
    # Only the tools changed the diff of a commit replayed without a stop.
    if reviewed:
        review_stamp.carry(commit, new)
    state["done"].append([commit, new])
    state["todo"].pop(0)
    save(state)


def stop(commit: str) -> None:
    subject = git("log", "-1", "--format=%s", commit)
    print(f"\nstopped at {commit[:10]} ({subject}).\n"
          "Fix the files named above: conflict markers, names the merge reported, a\n"
          "failed build or a function that no longer matches. Then `git add` them and run\n"
          "    python3 tools/rebase_onto_mainline.py --continue\n"
          "or give up with --abort.", file=sys.stderr)


def resume() -> int:
    if not STATE.exists():
        sys.exit("error: no rebase under way")
    state = json.loads(STATE.read_text())
    commit = state["todo"][0]
    # PROGRESS.md belongs to mainline; keep it out of this commit.
    git("reset", "-q", "HEAD", "--", "PROGRESS.md", check=False)
    git("checkout", "-q", "HEAD", "--", "PROGRESS.md", check=False)
    git("add", "-A", "config", "src", "include")
    if git("status", "--porcelain", "--untracked-files=no"):
        r = subprocess.run(["git", "commit", "-q", "-C", commit], cwd=ROOT)
        if r.returncode:
            stop(commit)
            return 1
        if review_stamp.needs_stamp("HEAD") and not review_stamp.is_stamped("HEAD"):
            print(f"{review_stamp.describe('HEAD')}: fixed by hand, so it needs a new tenets review"
                  " and stamp before it lands")
    else:
        print(f"{commit[:10]} has nothing left to commit after the fix; skipped")
    done(state, commit, reviewed=False)
    return replay(state)


def finish(state: dict) -> int:
    git("switch", "-q", "-C", state["branch"])
    git("branch", "-D", state["temp"])
    STATE.unlink()
    print(f"{state['branch']} now starts at mainline. Old -> new:")
    for old, new in state["done"]:
        touched = git("diff-tree", "--no-commit-id", "-r", "--name-only", new).split()
        warn = "  (carries PROGRESS.md!)" if "PROGRESS.md" in touched else ""
        print(f"  {old[:10]} -> {new}{warn}")
    return 0


def abort() -> int:
    if not STATE.exists():
        sys.exit("error: no rebase under way")
    state = json.loads(STATE.read_text())
    git("reset", "-q", "--hard")
    git("switch", "-q", state["branch"])
    git("reset", "-q", "--hard", state["orig"])
    git("branch", "-D", state["temp"])
    STATE.unlink()
    print(f"back on {state['branch']} at {state['orig'][:10]}")
    return 0


def parse_args() -> argparse.Namespace:
    """-h/--help print the module text and exit 0; unknown arguments exit 2.
    Runs before the worktree lock or any git state is touched."""
    parser = argparse.ArgumentParser(prog="rebase_onto_mainline.py", description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--continue", dest="resume", action="store_true")
    mode.add_argument("--abort", action="store_true")
    mode.add_argument("--onto", metavar="REV")
    return parser.parse_args()


if __name__ == "__main__":
    ARGS = parse_args()
    LOCK = lock_worktree()
    if ARGS.resume:
        sys.exit(resume())
    if ARGS.abort:
        sys.exit(abort())
    sys.exit(start())
