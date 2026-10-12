# Issues

A plain-file issue tracker, kept in git until the project moves to a real one. One file per issue, named `NNNN-short-name.md`. Numbers are never reused.

## Format

```markdown
# NNNN: Title

- **Status:** open | in progress | blocked | worked around | closed
- **Area:** build | tools | loop | decomp | docs | emulation
- **Opened:** YYYY-MM-DD
- **Milestone:** M1 | M2 | M3 | later

## Problem
What is wrong or missing, with exact error text when there is one.

## Notes
What we know, what was tried, links to code and other docs.

## Done when
How to tell it is fixed.
```

When an issue closes, set the status, add a line saying how it was resolved, and keep the file. Update the table below in the same commit.

## Index

| # | Title | Status | Area | Milestone |
|---|---|---|---|---|
| [0001](0001-exceptix-linker-warnings.md) | mwldarm warns about `.exceptix` for every C function | open | build | M1 |
| [0002](0002-shared-table-headers.md) | Agents create separate struct headers for the same table | in progress | loop | M1 |
| [0003](0003-pointer-table-callers.md) | `fn.py show` finds no callers for functions reached through pointer tables | closed | tools | M1 |
| [0004](0004-identical-functions.md) | Identical functions each cost a full agent run | open | loop | M1 |
| [0005](0005-names-to-confirm.md) | Names the agents guessed, to confirm or change | open | decomp | M1 |
| [0006](0006-emulator-boot-check.md) | The built ROM has not been run in an emulator | open | emulation | M3 |
| [0007](0007-dsd-init-abort.md) | `dsd init` aborts on AWRE | worked around | tools | later |
| [0008](0008-fn-add-blank-lines.md) | `fn.py add` leaves an extra blank line in `delinks.txt` | open | tools | M1 |
| [0009](0009-file-modified-after-diff.md) | An agent saw its C file change on disk after `fn.py diff` | open | tools | M1 |
| [0010](0010-build-log-noise.md) | dsd prints `[INFO]` lines on every build | open | build | M1 |
| [0011](0011-hook-builds-working-tree.md) | `pre-commit` builds the working tree, not the staged changes | open | build | later |
| [0012](0012-hard-coded-addresses.md) | Hard-coded addresses would break a shifted build | open | decomp | M3 |
| [0013](0013-arm7-from-objects.md) | ARM7 is copied from the ROM, not built from objects | closed | build | M2 |
| [0014](0014-unk-struct-consolidation.md) | `Unk_*` structs need merging and real names | open | decomp | M2 |
| [0015](0015-agent-usage-budget.md) | No usage budget per batch | open | loop | M1 |
| [0016](0016-field-renames-not-replayed.md) | `apply_commit.py` does not replay struct field renames | open | loop | M1 |
| [0017](0017-diff-reloc-names.md) | `fn.py diff` shows less than 100% for table accesses that match | closed | tools | M1 |
| [0018](0018-rh890-names-to-confirm.md) | Names from document 890 to confirm | open | decomp | M2 |
| [0019](0019-ov000-relocations.md) | Relocations marked `overlays(0,4)` that can only be ov000 | closed | tools | M3 |
| [0020](0020-apply-commit-header-conflicts.md) | `apply_commit.py` stops at shared-header conflicts | closed | loop | M1 |
| [0021](0021-auto-co-maps.md) | Name the Auto CO special case for Lightning Strikes and Ring of Fire | closed | decomp | M2 |
| [0022](0022-army-style-colour-comments.md) | Player.armyStyle and armyColour comments are wrong | closed | decomp | M2 |
| [0023](0023-mod-build-repo.md) | A separate mod build on top of the matching source | open | build | later |
| [0024](0024-brute-force-tool-leftovers.md) | What propagate.py, trivial.py and permute.py leave unmatched | open | tools | M1 |
| [0025](0025-landing-tool-faults.md) | Faults in the landing tools seen in batch 10 | closed | loop | M1 |
| [0026](0026-land-self-old-code.md) | land_self.py kept its old code after a replay and skipped the review check | closed | loop | M1 |
| [0027](0027-land-self-livelock.md) | Many land_self.py runs at once rebuilt after every landing | closed | loop | M1 |
| [0028](0028-header-merge-anonymous-members.md) | `header_merge.py` crashed on bit-fields named `pad_*` and dropped anonymous struct members | closed | loop | M1 |
| [0029](0029-header-merge-embedded-struct-size.md) | `header_merge.py` placed the commit's fields with mainline's struct sizes | closed | loop | M1 |
| [0030](0030-apply-commit-chained-renames.md) | `apply_commit.py` chained a commit's function renames | closed | loop | M1 |
| [0031](0031-agent-results-file-not-written.md) | Agents sometimes end without writing their results file | open | loop | M1 |
| [0032](0032-subagent-cap-counts-finished-agents.md) | The 20-subagent launch limit counts an agent that has already finished its work | open | loop | M1 |
| [0033](0033-collect-crash-empty-result.md) | `loop_state.py collect` crashed on a RESULT line with no value | closed | loop | M1 |
| [0034](0034-planner-overlaps-open-groups.md) | The planner puts functions of open groups into the next batch | open | loop | M1 |
| [0035](0035-family-groups-skip-propagate.md) | Family groups do not run `propagate.py` on their twins | open | loop | M1 |
| [0036](0036-agents-share-session-scratchpad.md) | Agents wrote scratch files into the shared session scratchpad | open | loop | M1 |
| [0037](0037-duplicate-structs-across-groups.md) | Groups that work on the same object define separate structs for it | open | decomp | M2 |
| [0038](0038-check-and-diff-disagree.md) | `fn.py check` reports BattleFighterTask_Setup 0x10 short while `fn.py diff` shows nothing | open | tools | M1 |
| [0039](0039-monitor-tasks-hold-subagent-slots.md) | Monitor tasks left running keep a finished agent's subagent slot | open | loop | M1 |
| [0040](0040-partials-job-result-format.md) | `loop_state.py collect` misreads the RESULT blocks of partials jobs | open | loop | M1 |
| [0041](0041-data-structure-helper.md) | No tool shows data structures the way `fn.py show` shows functions | closed | tools | M2 |
| [0042](0042-arm7-decompilation.md) | ARM7 has no C at all | open | decomp | later |
| [0043](0043-oomd-kills-session-with-20-agents.md) | systemd-oomd killed the orchestrator session with 20 agents running | worked around | loop | M1 |
| [0044](0044-landing-queue-wait-over-an-hour.md) | Landing queue waits of over an hour with 11 agents queued | open | loop | M1 |
| [0045](0045-collect-misreads-results.md) | `loop_state.py collect` misreads short and prose results files | open | loop | M1 |
| [0046](0046-replay-leaves-stale-moved-files.md) | Replay leaves the old copy of a moved source file | closed | loop | M1 |
| [0047](0047-review-stamp-without-review.md) | A tenets review stamp did not show that a review ran | closed | loop | M1 |
| [0048](0048-lost-attempt-tokens-not-counted.md) | Token totals leave out the attempts lost in a session crash | open | loop | M1 |
| [0049](0049-download-play-child.md) | The DS Download Play child program has no C and is not built | open | decomp | later |
| [0050](0050-asset-structure.md) | Game assets have no structure beyond the extracted file tree | open | build | M2 |
| [0051](0051-fn-add-includes-trailing-data.md) | `fn.py add` includes data after a function in its delinks.txt range | closed | tools | M1 |
| [0052](0052-partials-strategy.md) | The game partials need a better tool than permute.py and agent retries | open | tools | M2 |
| [0053](0053-duplicate-data-symbol-name.md) | Two data symbols with the same name break the arm9 checksum during a replay | open | loop | M1 |
| [0054](0054-arm7-unrelocated-pointers.md) | 25 ARM7 words that look like pointers have no relocation | open | decomp | M2 |
| [0055](0055-overlay-relocations-left.md) | 15 relocations still name several overlays | open | tools | M3 |
| [0056](0056-shifted-secure-area-crc.md) | A ROM with changed main code has a wrong secure-area CRC | open | build | M3 |
| [0057](0057-diff-below-100-for-matching-asm.md) | `fn.py diff` scores below 100% for hand-written asm whose ROM bytes match | open | tools | M1 |
| [0058](0058-dsd-arm-branch-unconditional.md) | dsd writes an `arm_branch` relocation in a delinked range as an unconditional `b` | worked around | tools | M1 |
| [0059](0059-replay-conflict-leaves-files-unstaged.md) | After a conflict in a replay, the resolved commit held one file and the next commit was dropped | open | loop | M1 |
| [0060](0060-dsd-symbol-sizes-too-small.md) | dsd symbol sizes too small for runtime functions, and tails split off as functions | open | tools | M1 |
| [0061](0061-msl-by-value-struct-frame.md) | MSL functions that mwccarm 2.0-72 cannot match because of the by-value struct frame | open | decomp | M2 |
| [0062](0062-protocheck-reports-left.md) | About 190 `protocheck.py` reports left in files touched only by renames | open | decomp | M2 |
| [0063](0063-trivial-arm7-tailcall-breaks-module-hash.md) | `trivial.py --arm7` output for `func_arm7_03807408` passes the fast check but breaks the arm7 module hash | closed | tools | M2 |
| [0064](0064-terrain-stats-start.md) | `Ov000_gTerrainStats` starts 8 bytes after the real table: names belong to the next terrain id | open | decomp | M2 |
| [0065](0065-tile-shape-names.md) | Tile tokens for shapes (coasts, bends) are numbers, not names | open | docs | M2 |
| [0066](0066-map-scripts.md) | Map event scripts are bytes in ov001, not in the map files | open | decomp | M2 |
| [0067](0067-partials-ranked-by-objdiff-percent.md) | Partials are ranked by objdiff's percentage, which favours register differences over a closer match | closed | loop | M2 |
| [0068](0068-permuter-asm-declarations.md) | permuter.py reports "0 outputs" on files with `asm` declarations, without running | closed | tools | M2 |
