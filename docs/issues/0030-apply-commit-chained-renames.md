# 0030: apply_commit.py chained a commit's function renames

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

Batch 14, group o6m7: one commit renamed `Ov006_CountRiverNeighbors` (0x023620cc) to `Ov006_CountRiverOrBridgeNeighbors` and gave the old name to `func_ov006_023622bc`. Every `land_self.py` replay gave the callers of 0x023622bc the name `Ov006_CountRiverOrBridgeNeighbors`, which ended in a conflict or a "Multiply-defined" link error. The agent landed the two functions in one commit by hand (82c4e8517).

Two causes in `apply_commit.py`:

- `rename_in_sources` applied each rename to every file in `src/` and `include/` after the merge, also to the files the commit had just written. Those already had the new names, so `A -> B` turned the new `A` (meaning C) into `B`.
- Renames ran one after the other, in `symbols.txt` order. When the line `C -> A` came first, mainline's calls of C became A, then B. `apply_symbols` also found each line by name only, so it could rename the wrong line.

## Fixed (2026-10-10)

`header_merge.rename_words` renames all names in one pass, so `{A: B, C: A}` cannot chain. `rename_in_sources` uses it and skips the files the commit changed (`merge_file` already renamed mainline's side of those before merging). `apply_symbols` finds every renamed line by name and address before changing any.

Tests: `make test` (`ChainedRenameTest` in `tools/tests/test_landing.py`, symbols in address order and in reverse order).
