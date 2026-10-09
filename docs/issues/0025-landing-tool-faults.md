# 0025: Faults in the landing tools seen in batch 10

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-09
- **Milestone:** M1

## Problem

Batch 10 ran 43 agents that landed their own work. Each fault below cost an agent or the orchestrator hand work. None broke mainline: `land_self.py` only fast-forwards after `make release` passes.

## apply_commit.py and header_merge.py

- A field rename from the header merge is applied by bare name to the agent's C files, so reads of a field with the same name in another struct get renamed too (`mapWidth` and `mapHeight` of `unk_1c4` became `unk_44`/`unk_48` while merging `Unk_020ffa50`). Renames must be scoped to the struct.
- New `#define` lines were dropped on a clash with a neighbouring define (`TERRAIN_*` next to `TERRAIN_LAB`). Defines should merge as a union.
- Lines removed from `relocs.txt` are not replayed; the renames agent applied that commit with `git cherry-pick`.

## rebase_onto_mainline.py and land_self.py

- After a replay rewrites `delinks.txt`, the next `make` can fail with a missing `_dsd_gap@...o`, and a stale object can survive. Touching `delinks.txt` and running `make` twice got past it. Likely a ninja dependency or file-time problem.
- A `land_self.py` run that outlives the Bash tool timeout keeps running in the background. A second run started on the same worktree then overlaps it and the replayed build fails. `land_self.py` should refuse to start while `rebase-onto-mainline.json` exists or another replay runs for that worktree.
- One agent ran `pkill -f tools/land_self.py` to clean up its own runs and stopped other agents' landings. The agent prompt now says never to stop processes the agent did not start.
- Waits written as `pgrep -f <pattern>` loops match their own command line and never end.

## Done when

The header merge scopes renames and unions defines, `relocs.txt` removals replay, the first build after a replay succeeds, and `land_self.py` refuses to overlap a running replay.
