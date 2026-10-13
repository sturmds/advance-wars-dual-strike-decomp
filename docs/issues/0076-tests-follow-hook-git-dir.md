# 0076: `make test` run from a git hook wrote into the real repository

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-13
- **Milestone:** M1

## Problem

While an agent added `make test` to `make release`, 29 commits by committer `test` (`base`, `Add a`, `Add b`, `agent change`) went onto the real `mainline`, branches `a`, `b` and `agent` and two temporary worktrees were registered in the real repository, the agent's own branch got a `base` commit, and `.git/config` got `core.bare=true`, a temporary `core.hooksPath` and `user.name=test`. Those are the values `tools/tests/test_landing.py` sets on its throwaway repo.

## Cause

Git exports `GIT_DIR`, `GIT_INDEX_FILE` and other variables to hooks so that git commands in a hook find the repository. `hooks/pre-commit` runs `make release`; with `make test` inside it, the tests inherited `GIT_DIR` pointing at the committing worktree's git directory. Every git command in `test_landing.py` follows `GIT_DIR` before its `cwd=`, so `git init` and `git config` rewrote the real config (with `GIT_DIR` set and no `GIT_WORK_TREE`, `init` marks the repository bare), the first commit went to the worktree's branch, and the rest went to the real `mainline`. Runs of `make test` from a shell did no harm because no `GIT_` variable was set there. This cause comes from the evidence and githooks(5); the run was not reproduced.

## Fixed (2026-10-13)

- The `test` target in the `Makefile` runs each test under `env -u` for every `GIT_` variable in the environment except `GIT_EDITOR`.
- `test_landing.py` deletes the same variables from its own environment when it starts, so a direct run from a hook is safe too, and the tools it starts inherit that environment.
- After `git init`, `Repo` checks that `git rev-parse --absolute-git-dir` lies inside the test's temporary directory, and stops the whole run (exit 3) if it does not.
- The other tests do not create repositories; `test_loop_state.py` only reads this checkout through `tools/loop_state.py`.
