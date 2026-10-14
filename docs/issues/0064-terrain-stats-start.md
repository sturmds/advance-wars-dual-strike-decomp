# 0064: `Ov000_gTerrainStats` starts 8 bytes after the real table

- **Status:** closed
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

`include/game/terrain.h` puts `TerrainStats` at 0x022f4ccc with `defenseStars` first and the name and description at +0x10 and +0x14. Read that way, entry `t` holds the stars of terrain `t` but the name of terrain `t + 1`: entry 1 has 1 star (Plain) and the name "River". `SecondScreen_DrawTerrainDetails` reads the names through `data_ov000_022f4cc4` and `data_ov000_022f4cc8`, 8 bytes before the table, to make up for it.

The entries start at 0x022f4cc4: name (+0), description (+4), stars (+8), then the three fields that are now `unk_04`, `unk_08` and `unk_0c`. With that layout, the names line up with the terrain ids (1 Plain, 9 "Sea", 23 Crystal, 24 G Bolt, 25 Laser, 27 G Bolt, 30 Deathray, 31 Blocked).

## Notes

- Found while naming terrain ids for the map data (`docs/map-terrain.md`).
- Moving the symbol means: a new layout in `terrain.h`, a symbol at 0x022f4cc4 in `config/AWRE/arm9/overlays/ov000/symbols.txt`, and checking every user of `Ov000_gTerrainStats` (`ds.py field TerrainStats.defenseStars`) and of the two folded symbols. `Ov000_gUnitStats` starts right after the table, so check where the last entry ends.

## Done when

`TerrainStats` starts at 0x022f4cc4 with the name first, the users read it by field, and `make release` passes.

## Resolution

2026-10-14 (shift-tables-2): the entries start at 0x022f4cb8, one more word back than this issue said, and the table holds exactly one entry per terrain id (32 of 0x18 bytes, all zero for terrain 0). `TerrainStats` is picture, palette, a word that is always 0, name, description, defense stars. `Ov000_gTerrainStats` now names the table at 0x022f4cb8, written from a struct row in `data_inc.txt`; the four symbols the code loads (0x022f4cbc, 0x022f4cc4, 0x022f4cc8 and the old `Ov000_gTerrainStats` at 0x022f4ccc, now `data_ov000_022f4ccc`) are aliases of `[0].palette`, `[0].name`, `[0].description` and `[0].defenseStars`, and the code reads the fields.
