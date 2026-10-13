# 0073: Agents share one scratchpad, and a wait loop outlives its agent

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

- The scratchpad directory is one for the whole session, so concurrent agents write to the same files. `b38/ov002_023549c8` reported that it may have overwritten another agent's `v/base.c`. The orchestrator told the running agents to use their own subfolder at 23:36, but the job prompts did not say so.
- `b38/a7_027c6020` left a `pgrep -f` wait loop running after it ended. The pattern matched the loop's own command line, so it never exited. The orchestrator killed it at 23:58.
- The prompt files under `work/loop-prompts/` were rewritten to a home path that does not exist on this machine. Agents fell back to the paths in their launch message. A prompt should name `work/` of the main checkout without a home directory.

## Fix

Not made. Add one line to the group prompt template (scratch files in `<scratchpad>/<group>/`, no `pgrep -f` self-matching waits) and make the prompt writer use repository-relative paths.
