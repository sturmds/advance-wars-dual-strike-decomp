# 0045: `loop_state.py collect` misreads short and prose results files

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

In batch 18 and the batch 19 relaunches, `collect` read these files wrongly:

- `b18/s03` wrote prose with no `RESULT:` blocks. Set to done by hand after reconcile counted 15/15 in C.
- `b18/s13` has one line saying all 15 matched, with a landing hash. The group shows "landed none".
- `b19/s06` wrote a one-line `RESULT`; `collect` called it failed and reconcile closed it.
- `partials18` landed nothing and `collect` called it failed; it was set to done by hand (0 of 8 improved is a valid result for a partials job).

Each time the notes in the file (partial percentages, suggested names) were not read into the state.

## Workaround

`loop_state.py reconcile` counts the functions now in C and the orchestrator sets the status by hand.

## Ideas

- Accept a one-line summary form (`all N matched, LANDED: <hash>`).
- Treat a partials job with `0/N` and no commits as done, not failed.
- Related: 0031 (files not written), 0040 (partials RESULT format).
