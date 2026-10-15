# 0095: wt_setup.sh copies the main checkout's unpacked assets, edits included

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-15
- **Milestone:** later

## Problem

`tools/wt_setup.sh` copies `extracted/AWRE/assets/maps`, `text` and `gfx/files` from the main checkout into a new worktree. When the main checkout is on a branch that edits or adds unpacked files (a mod branch), the worktree gets those edits, and `make release` there fails with `error: build/AWRE.nds does not match the retail ROM` although the worktree's branch changes nothing.

## Notes

- Seen on a worktree of `mainline` made while the main checkout was on a mod branch: removing the three copied directories and running `make setup` gave a passing `make release`.
- The copies save the unpack time. A copy could be checked against the ROM (the unpack tools write only changed files, so `make setup` after the copy would bring back the ROM's files but keep added ones such as a new text bank or picture).

## Done when

A worktree made by `wt_setup.sh` builds the ROM of its own branch whatever branch the main checkout is on.
