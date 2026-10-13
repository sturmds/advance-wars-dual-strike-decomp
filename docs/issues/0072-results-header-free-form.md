# 0072: Agents write RESULT headers that `collect` cannot read

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

In batch 38 the triage jobs `triage-wtsetup-extracted` and `leads38`, and the group `b38/ov003_02361c7c`, wrote a bare `RESULT` or a free-form first line instead of `RESULT: <done|partial|failed>`. The orchestrator fixed each header by hand before `loop_state.py collect` could close the group. Same family as issue 0040 (partials jobs) and issue 0031 (no results file).

## Fix

Not made. Options: have `collect` accept `RESULT` followed by any text and take the outcome from a `STATUS:` or `NOTES:` line, or add a `loop_state.py check-result FILE` that the job prompts tell agents to run as their last step.
