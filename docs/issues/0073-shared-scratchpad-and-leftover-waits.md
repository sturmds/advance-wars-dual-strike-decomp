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

## Batch 39

The same fault came back: finished agents left `pgrep -f` wait loops, and the orchestrator killed them. One kill was at first thought to hit a running agent; it did not. The rule is still missing from the group prompt template.

## Batches 41 and 42

Three more `pgrep -f` wait loops (b42/ctx, the permuter runs of b42/uipd-shape and b42/ctx) were left behind and killed by the orchestrator. The prompt template still lacks the rule.
