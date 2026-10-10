# 0026: land_self.py kept its old code after a replay and skipped the review check

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-09
- **Milestone:** M1

## Problem

The review stamp check (101844a6f) landed in `land_self.py`. Group b11/o2m1 then landed a4d571555..a0dd36b8d without a tenets review. Its worktree's `land_self.py` predated the check. The replay onto mainline brought the new `tools/land_self.py` into the worktree, but the Python process had already loaded the old file and went on to build and fast-forward with it.

`python3 tools/review_stamp.py --help` also failed: the argument went to `git diff-tree` as a revision.

## Fixed (2026-10-09)

- `land_self.py` reads `tools/land_self.py` and `tools/review_stamp.py` at start. After each replay, if either file changed, it starts again from the replayed copy (`os.execv`), before `make release` and before the fast-forward. The new run takes the worktree lock again and does the stamp check.
- `review_stamp.py` parses its arguments with argparse, so `--help` works.

A run that started before this fix still runs the old code to the end. Runs started after it pick up later changes to the check.

Tests: `make test` (`LandSelfReexecTest` in `tools/tests/test_landing.py`).
