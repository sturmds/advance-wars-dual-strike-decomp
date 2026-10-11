# 0040: `loop_state.py collect` misreads the RESULT blocks of partials jobs

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

`jobs/partials.md` has agents write one block per function that starts `RESULT: <function name>` and carries the outcome in a `STATUS:` line. `cmd_collect` reads the word after `RESULT:` as the outcome, so it sees a function name and cannot tell matched from no gain. The orchestrator closed partials14 (landed ddf469b3) and partials16 (landed 43ec842cc) by hand. renames jobs are also set by hand.

## Fix

Either change `jobs/partials.md` to the usual `RESULT: matched|partial|failed` plus `FUNCTION:` form, or teach `cmd_collect` to read `STATUS:` when `RESULT:` is not one of the known words. Add a case to `tools/tests/test_loop_state.py`.

## Done when

A partials job with the format above is closed by `collect` without a hand edit.
