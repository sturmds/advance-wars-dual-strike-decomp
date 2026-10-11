# 0034: The planner puts functions of open groups into the next batch

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

`plan12` did not exclude the functions of batch 11 groups that were still running. `b12/o2s0` (13 functions, all from `b11/o2s0`) used 60k tokens before the orchestrator caught it, and `b12/o0s0` (also from `b11/o0s0`) used 56k before it was told to stop. Neither landed anything. Later planners (`plan13`) were told to exclude open groups in their prompt, which worked.

## Workaround

The planner prompt names `python3 tools/loop_state.py claimed` and tells it to skip those functions.

## Ideas

- Have `loop_state.py` refuse to add a group whose functions overlap a group that is not closed.
