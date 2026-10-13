# 0080: The worktree guard refuses commands the loop needs

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

Agents in worktrees run under a guard that refuses commands it cannot show to stay inside the worktree. In batches 41 to 43 it refused:

- the Write tool for the results file under the main checkout's `work/loop/results/` (b41/os copied the file with `cp` instead),
- `tools/wt_setup.sh` and commit messages passed as a long `-m` string (b41/svc7 made the links by hand and used `git commit -F`),
- any shell line that mixes `git` with a loop, a pipe, a computed variable or a command substitution (wrapup41 split its commands one by one).

Each refusal costs a few turns, and a missing results file is what issue 0031 describes.

## Fix

Not made. Put the permitted forms in the group prompt template: results file written by `cp` from the scratchpad, one command per Bash call, `git commit -F` for long messages.
