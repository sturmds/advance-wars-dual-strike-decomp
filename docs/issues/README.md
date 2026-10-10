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
| [0003](0003-pointer-table-callers.md) | `fn.py show` finds no callers for functions reached through pointer tables | open | tools | M1 |
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
