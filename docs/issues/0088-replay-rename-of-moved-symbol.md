# 0088: The landing replay renames a symbol that moved to a new address

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-14
- **Milestone:** M2
- **Related:** 0087 (found during its work)

## Problem

`apply_commit.py` (`rename_in_sources`, used by `land_self.py`'s replay) reads a symbol whose name now belongs to another address as a rename of the symbol at the old address. When a commit moves a name, the replay rewrites files the commit did not touch.

Seen 2026-10-14 (agent shift-tables-2): its commit moved `Ov000_gTerrainStats` from 0x022f4ccc to 0x022f4cb8, the real start of the table (issue 0064), and gave 0x022f4ccc the alias name `data_ov000_022f4ccc`. The replay then rewrote the two readers of the table in `src/game/main/skills/` from `Ov000_gTerrainStats` to `data_ov000_022f4ccc`, which no C declares, and the final `make release` failed. Commit 633825d5b fixed the two files by hand.

## Fix

Skip a rename when the old name is still a symbol after the commit (it moved, it was not renamed), and add the case to `tools/tests/test_landing.py`.

## Done when

A commit that moves a name to another address replays without touching other files.

## Fixed (2026-10-14)

`apply_symbols` still renames the line in `symbols.txt`, but it no longer passes a rename on to the sources when the commit gives the old address its address name (`func_...`, `data_...`) and the old name is still a symbol after the commit: the name moved. A rename where the old name disappears, and a swap of two names between two addresses, still rewrite the sources, so they keep pointing at the same addresses (issue 0030's case `Count -> CountAny`, `func_02000a00 -> Count` stays a rename).

Tests: `MovedNameTest` in `tools/tests/test_landing.py` (name moved to an address that had a symbol, name moved to a new address, name given up for the address name, two names swapped).
