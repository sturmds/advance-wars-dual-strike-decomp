# 0039: Monitor tasks left running keep a finished agent's subagent slot

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

On 2026-10-10 22:20 the launches of `b18/x04` and `b18/l01` were refused at the subagent limit. `b17/s13` had reported its result but still had four Monitor tasks running, and they kept its slot counted as live. This is a cause of issue 0032.

## Workaround

The orchestrator retries the launch on the next notification. Agent prompts should say to stop every Monitor task (TaskStop) before ending.

## Ideas

- Add "stop your monitors" to the last step of the `decomp-function` skill.
- Have the orchestrator list a finished agent's remaining tasks and stop them.
