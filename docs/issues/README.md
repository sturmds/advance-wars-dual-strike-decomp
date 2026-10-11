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
| [0006](0006-emulator-boot-check.md) | The built ROM has not been run in an emulator | blocked | emulation | M3 |
| [0007](0007-dsd-init-abort.md) | `dsd init` aborts on AWRE | worked around | tools | later |
| [0008](0008-fn-add-blank-lines.md) | `fn.py add` leaves an extra blank line in `delinks.txt` | open | tools | M1 |
| [0009](0009-file-modified-after-diff.md) | An agent saw its C file change on disk after `fn.py diff` | open | tools | M1 |
| [0010](0010-build-log-noise.md) | dsd prints `[INFO]` lines on every build | open | build | M1 |
| [0011](0011-hook-builds-working-tree.md) | `pre-commit` builds the working tree, not the staged changes | open | build | later |
| [0012](0012-hard-coded-addresses.md) | Hard-coded addresses would break a shifted build | open | decomp | M3 |
| [0013](0013-arm7-from-objects.md) | ARM7 is copied from the ROM, not built from objects | open | build | M2 |
| [0014](0014-unk-struct-consolidation.md) | `Unk_*` structs need merging and real names | open | decomp | M2 |
| [0015](0015-agent-usage-budget.md) | No usage budget per batch | open | loop | M1 |
| [0016](0016-field-renames-not-replayed.md) | `apply_commit.py` does not replay struct field renames | open | loop | M1 |
| [0017](0017-diff-reloc-names.md) | `fn.py diff` shows less than 100% for table accesses that match | closed | tools | M1 |
| [0018](0018-rh890-names-to-confirm.md) | Names from document 890 to confirm | open | decomp | M2 |
| [0019](0019-ov000-relocations.md) | Relocations marked `overlays(0,4)` that can only be ov000 | open | tools | M3 |
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
| [0046](0046-replay-leaves-stale-moved-files.md) | Replay leaves the old copy of a moved source file | open | loop | M1 |
| [0047](0047-review-stamp-without-review.md) | A tenets review stamp did not show that a review ran | closed | loop | M1 |
