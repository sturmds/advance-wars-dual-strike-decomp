# 0015: No usage budget per batch

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

Each decomp agent costs about 45–60k tokens on small functions, more on larger ones. The usage limit was reached twice on the night of 2026-10-06 while batches ran. The loop has no way to size a batch to a budget.

## Notes

- Record tokens per agent in the batch log (`docs/loop.md`) to get a real average by function size.
- Let sturmds set a budget per batch; the orchestrator picks a batch that fits.
- Spend budget where it counts: `config/AWRE/interesting.txt` first, then callers of matched functions, and skip twins (issue 0004).

## Done when

Each batch states its budget and its actual cost in the log.
