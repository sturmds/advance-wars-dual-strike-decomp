# 0069: wt_setup.sh did not provide extracted/ to a new worktree

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-12
- **Milestone:** M2

## Problem

Since `make setup` unpacks the maps into `extracted/` (ec51c0ea9), the first `make` in a fresh agent worktree failed because `extracted/` is untracked and `tools/wt_setup.sh` linked only `orig/AWRE.nds` and `.cache`. Agents ran `make setup` in the worktree to get past it. Found by plan37 and b38/ov002_02352754.

## Fix

`wt_setup.sh` now links `extracted/` from the main checkout and stops with a message when the main checkout has none. `.gitignore` listed `/extracted/`, which does not match a symlink, so the link showed as untracked; it is now `/extracted`.

## Open

`.claude/` is gitignored, so skills are missing from worktrees. Claude Code agents still see the skills through the main checkout's session; no change made.
