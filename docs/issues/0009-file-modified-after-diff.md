# 0009: An agent saw its C file change on disk after `fn.py diff`

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

The func_02002620 agent reported that its C file was "modified since read" between two of its own writes, with no other editor. A second agent saw the same after `fn.py rename`, which is expected (rename rewrites sources). The first case is not explained.

## Notes

- Things that write under `src/`: `fn.py rename` (word replace in all `src/` and `include/` files), `apply_commit.py` (main checkout only). `fn.py diff` runs `configure.py` and ninja, neither of which should touch sources.
- mwccarm with `-gccdep -MD` writes the `.d` file next to the object in `build/`, not next to the source. Worth confirming with `stat` before and after a `fn.py diff`.
- It may be the agent harness's own file tracking, not a real change.

## Done when

Either the cause is found and fixed, or a test shows `fn.py diff` leaves `src/` untouched.
