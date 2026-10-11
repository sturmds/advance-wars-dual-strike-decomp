# 0036: Agents wrote scratch files into the shared session scratchpad

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

Agents in worktrees write temporary files to the session scratchpad, which every agent of the session shares. In batch 13 three agents (`o0l0` of batch 11, then two more) reported finding or copying another agent's file there. One was caught only before its commit.

## Workaround

Launch messages now say to keep scratch files in the worktree's `build/` directory. An agent that ignores this can still collide.

## Ideas

- Give each worktree its own scratch directory in `tools/wt_setup.sh` and name it in the skill.
- Have the skill refuse a path outside the worktree for generated C.
