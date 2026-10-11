# 0043: systemd-oomd killed the orchestrator session with 20 agents running

- **Status:** worked around
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

At 01:05 on 2026-10-11 `systemd-oomd` killed the tmux scope that held the orchestrator session (23.6 GB of 30 GB in use, 20 agents running, each with its own worktree, build and compiler processes). Every agent died with it. Work that was not committed and landed was lost; some groups redid part of their functions (batch 19 `s02`, `l00`, `l01` report functions "from the lost attempt").

## Workaround

A new session took the lease at 08:24 and the agent cap was lowered from 20 to 12.

## Ideas

- Have the orchestrator check free memory before each launch and hold launches below a floor.
- Run the session in a scope that `systemd-oomd` does not pick first, or give agents a memory limit.
- Tell agents to commit after each matched function so a kill loses less.
