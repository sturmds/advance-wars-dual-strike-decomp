# 0031: Agents sometimes end without writing their results file

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

In batch 11, four groups ended without a usable results file:

- `b11/fam3`: the first write was refused by permissions; the agent wrote it when asked.
- `b11/o2m3`: no file. The orchestrator closed the group by hand after seeing 7/7 functions in C on mainline.
- `b11/o1s2`: the file still said `PENDING LANDING` after the agent stopped. Closed by hand as 11/13.
- `b11/o2s3`: the file said partial 11/13 with "landed none" while the agent still had landing work running.

Later, `partials14` wrote RESULT blocks without the colon after `RESULT`, and the parser skipped them.

## Workaround

`loop_state.py reconcile` counts the functions now in C and closes a group by hand. It does not recover the group's notes (partial percentages, tips).

## Ideas

- Have `land_self.py` write the group's results file when it prints `LANDED`, from the commits it landed.
- Make `loop_state.py collect` accept `RESULT` with or without the colon.
