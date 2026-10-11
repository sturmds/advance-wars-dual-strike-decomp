# 0048: Token totals leave out the attempts lost in a session crash

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

When systemd-oomd killed the session at 01:05 on 2026-10-11 (issue 0043), 20 batch 19 groups lost their agents. The relaunched groups took over the lost attempts' commits (for example `b19/s09` landed 15 of 15 from the old branch for 47k tokens). `loop_state.py report` counts only the tokens of the agent that finished, so the lost attempts' tokens are missing from the totals. The batch 19 figure of 53 functions per 1M tokens is too high by an unknown amount, and the groups that were relaunched look cheaper than they were.

## Workaround

Read the batch 19 row of `docs/loop.md` as an upper bound on functions per 1M tokens for the groups that were relaunched.

## Ideas

- Keep `agent_tokens` for a lost agent when the group is relaunched, if the lost session's transcript still shows its total.
- Mark relaunched groups in `report` so their rate is not mixed with the rest.
- Related: 0043.
