# 0044: Landing queue waits of over an hour with 11 agents queued

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

On 2026-10-10 at 22:14, 11 `land_self.py` runs were waiting in the landing queue, and `b17/s13` reported a wait of more than an hour. Each landing runs `make release` under the queue lock (about 1.8 minutes in batch 16), so 11 waiting runs cost about 20 minutes each time, and a run that loses to a newer mainline replays again. Agents sit idle in the queue and keep their subagent slot (see 0039).

## Workaround

None. The orchestrator launches new groups as slots free up, which keeps the queue long.

## Ideas

- Hold launches when the queue is longer than a set length.
- Land several commits from one agent per queue turn (one `make release` for the group).
