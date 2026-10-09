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

Exit codes: 0 landed, 1 conflict to fix, 2 build failed, 3 gave up.
"""

import fcntl
import subprocess
import sys
import time
from pathlib import Path

LANDED, CONFLICT, BUILD_FAILED, GAVE_UP = 0, 1, 2, 3
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


def run(*cmd) -> int:
    return subprocess.run(list(cmd), cwd=ROOT).returncode


def rebase() -> tuple[int | None, str]:
    """Step 1. Returns an exit code (or None to go on) and the mainline
    commit the branch now starts from."""
    if Path(git("rev-parse", "--path-format=absolute", "--git-path", "rebase-onto-mainline.json")).exists():
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
