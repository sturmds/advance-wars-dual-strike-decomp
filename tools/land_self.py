#!/usr/bin/env python3
"""Land this worktree's branch on mainline, from inside the agent's worktree.

    land_self.py

Run it as the last step, once the work is committed. With 20 agents at
once, one orchestrator replaying every commit becomes the bottleneck, so
each agent lands its own branch:

1. Replay the branch onto the current mainline (tools/rebase_onto_mainline.py).
   If that stops, fix the files, run `rebase_onto_mainline.py --continue`,
   then run this again.
2. Run `make release`.
3. Write PROGRESS.md for the rebased tree and amend it into the branch's
   last commit, so mainline's copy stays current. This is the only amend
   in the loop.
4. Under an exclusive lock (<git common dir>/land.lock), check that
   mainline has not moved since step 1, and fast-forward the main
   checkout's mainline to the branch. If mainline moved, start again at
   step 1 (at most 15 tries). If the main checkout has uncommitted changes,
   wait for it (30 s steps, at most 10 minutes).

It never forces and never pushes.

It refuses to start (exit code 4) while a replay is open in this worktree
(rebase-onto-mainline.json exists: finish it with --continue or --abort)
or while another land_self.py or rebase_onto_mainline.py runs in this
worktree. A run that outlived a tool timeout keeps running in the
background; wait for it with `flock <lock file> true` (the message names
the file) instead of starting a second one, and never stop it with pkill.

Exit codes: 0 landed, 1 conflict to fix, 2 build failed, 3 gave up,
4 busy (a replay is open or another run is under way in this worktree).
"""

import fcntl
import os
import subprocess
import sys
import time
from pathlib import Path

LANDED, CONFLICT, BUILD_FAILED, GAVE_UP, BUSY = 0, 1, 2, 3, 4
TRIES = 15
WAIT_STEP, WAIT_MAX = 30, 600


def git(*args, cwd=None, check=True) -> str:
    r = subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True)
    if check and r.returncode:
        print(f"error: git {' '.join(args)}: {r.stderr.strip()}", file=sys.stderr)
        sys.exit(GAVE_UP)
    return r.stdout.strip()


ROOT = Path(git("rev-parse", "--show-toplevel"))
COMMON = Path(git("rev-parse", "--path-format=absolute", "--git-common-dir"))
MAIN = COMMON.parent  # the main checkout, where mainline is checked out
STATE = Path(git("rev-parse", "--path-format=absolute", "--git-path", "rebase-onto-mainline.json"))
# Per worktree, held for the whole run; rebase_onto_mainline.py takes it too.
WORKTREE_LOCK = Path(git("rev-parse", "--path-format=absolute", "--git-path", "land-self.lock"))


def run(*cmd) -> int:
    # The replay started here sees that this run holds the worktree lock.
    return subprocess.run(list(cmd), cwd=ROOT, env={**os.environ, "LAND_SELF_LOCK_HELD": "1"}).returncode


def lock_worktree():
    """Take this worktree's lock, or None if another run holds it. The file
    stays open, and locked, until the process exits."""
    f = open(WORKTREE_LOCK, "a+")
    try:
        fcntl.flock(f, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        f.seek(0)
        holder = f.read().strip()
        f.close()
        print(f"error: another land_self.py or rebase_onto_mainline.py runs in this worktree"
              f"{' (' + holder + ')' if holder else ''}. Do not start a second one and do not"
              f" stop it. Wait for it with\n    flock {WORKTREE_LOCK} true\nthen read its output"
              f" or `git log -1` to see whether it landed.", file=sys.stderr)
        return None
    f.seek(0)
    f.truncate()
    f.write(f"pid {os.getpid()}, {' '.join(sys.argv)}\n")
    f.flush()
    return f


def rebase() -> tuple[int | None, str]:
    """Step 1. Returns an exit code (or None to go on) and the mainline
    commit the branch now starts from."""
    if STATE.exists():
        print("a rebase is under way: finish it with rebase_onto_mainline.py --continue, then rerun this")
        return CONFLICT, ""
    onto = git("rev-parse", "refs/heads/mainline")
    if run(sys.executable, "tools/rebase_onto_mainline.py", "--onto", onto):
        return CONFLICT, onto
    return None, onto


def write_progress() -> int | None:
    """Step 3: amend a new PROGRESS.md into the last commit."""
    if run(sys.executable, "tools/progress.py", "--write", "PROGRESS.md"):
        return BUILD_FAILED
    if not git("status", "--porcelain", "--", "PROGRESS.md"):
        return None
    git("add", "PROGRESS.md")
    # The hooks run again here, so the amended commit also passes make release.
    if run("git", "commit", "-q", "--amend", "--no-edit"):
        return BUILD_FAILED
    return None


def main_checkout_ready() -> bool:
    """Wait until the main checkout is on mainline with no uncommitted changes."""
    waited = 0
    while True:
        branch = git("branch", "--show-current", cwd=MAIN)
        dirty = git("status", "--porcelain", "--untracked-files=no", cwd=MAIN)
        if branch == "mainline" and not dirty:
            return True
        if waited >= WAIT_MAX:
            print(f"the main checkout {MAIN} is " + ("not on mainline" if branch != "mainline" else "not clean")
                  + f" after {WAIT_MAX} s; giving up", file=sys.stderr)
            return False
        print(f"the main checkout is busy; waiting {WAIT_STEP} s")
        time.sleep(WAIT_STEP)
        waited += WAIT_STEP


def fast_forward(onto: str, branch: str) -> str | None:
    """Step 4. Mainline's new hash, or None if mainline moved since the rebase."""
    with open(COMMON / "land.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        if git("rev-parse", "refs/heads/mainline") != onto:
            return None
        r = subprocess.run(["git", "-C", str(MAIN), "merge", "-q", "--ff-only", branch],
                           capture_output=True, text=True)
        if r.returncode:
            print(f"error: fast-forward failed: {r.stderr.strip()}", file=sys.stderr)
            sys.exit(GAVE_UP)
        return git("rev-parse", "refs/heads/mainline")


def main() -> int:
    sys.stdout.reconfigure(line_buffering=True)  # progress shows up in a log while it waits
    if STATE.exists():
        print("error: a replay is open in this worktree. Finish it with\n"
              "    python3 tools/rebase_onto_mainline.py --continue\n"
              "(or --abort), then run land_self.py again.", file=sys.stderr)
        return BUSY
    lock = lock_worktree()
    if lock is None:
        return BUSY
    branch = git("branch", "--show-current")
    if not branch or branch == "mainline" or ROOT == MAIN:
        print("error: run this in an agent worktree, on its own branch", file=sys.stderr)
        return GAVE_UP
    if git("status", "--porcelain", "--untracked-files=no"):
        print("error: commit your work first", file=sys.stderr)
        return GAVE_UP
    for attempt in range(1, TRIES + 1):
        code, onto = rebase()
        if code is not None:
            return code
        if git("rev-parse", "HEAD") == onto:
            print("nothing to land: the branch has no commits of its own")
            return GAVE_UP
        if run("make", "release"):
            return BUILD_FAILED
        code = write_progress()
        if code is not None:
            return code
        if not main_checkout_ready():
            return GAVE_UP
        new = fast_forward(onto, branch)
        if new is not None:
            print(f"LANDED: {new}")
            return LANDED
        print(f"mainline moved during try {attempt}; rebasing again")
    print(f"mainline kept moving; gave up after {TRIES} tries", file=sys.stderr)
    return GAVE_UP


if __name__ == "__main__":
    sys.exit(main())
