# 0084: Land several waiting branches with one release

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-14
- **Milestone:** later

## Problem

`land_self.py` now runs one `make release` per landing instead of one per replayed commit. Landings still take turns in `<git common dir>/land-queue.lock`, so 7 queued agents cost 7 releases in a row, about 12 minutes, even when their changes do not touch each other.

## Design (not built)

A run that holds the queue takes the branches waiting behind it as a "train" and checks them with one release:

1. Each waiting `land_self.py` writes a request file, `<git common dir>/land-train/<branch>`, with its worktree path and the branch head it ran its review check on, then blocks on the queue as now.
2. The holder reads the requests, and for each branch in order replays that branch's commits onto the tip of the train in a temporary worktree of its own (never in the other agent's worktree, which its own `land-self.lock` protects), with the same review-stamp check before and after the replay as for its own branch.
3. Any stop (a conflict, a missing stamp, a branch head that moved since its request) drops that branch from the train; its own run lands it alone when its turn comes.
4. One `make release` on the train's final tree. If it fails, the holder lands nothing of the others and falls back to landing its own branch alone; each waiting run then lands alone.
5. On a pass, one fast-forward under `land.lock`, then each request file gets the result (`LANDED <hash>`), and each waiting run, on its turn, finds its branch already on mainline, moves its branch to the replayed commits and reports `LANDED`.

## Why not now

Step 5 changes another agent's branch from outside its worktree, and a run that is killed between the fast-forward and the result files leaves waiting runs that must tell "landed by the train" from "mainline moved". Both need tests of their own across several processes. A failing train release also turns one agent's mistake into several agents' delay unless the step back to single landings works. The single-branch landing is simple and safe; build this when the queue, not the release per landing, is the bottleneck.
