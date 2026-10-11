# 0032: The 20-subagent launch limit counts an agent that has already finished its work

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

2026-10-09 18:51: the launch of `b11/o2x2` was refused with the 20-subagent limit although the orchestrator counted 19 running groups. The agent of `b11/o1s1` had landed all its commits but still counted as live. The orchestrator retried on the next tick (23 minutes later), so one slot sat empty for that time.

A related case at 2026-10-10 20:22: `triage-link-time` was launched at 21 of 20 on purpose, by skipping the next free slot.

## Workaround

Retry on the next tick, or after the stale agent exits.

## Ideas

- Count a group as closed once its results file says LANDED, and retry the launch a few minutes later instead of waiting a full tick.
