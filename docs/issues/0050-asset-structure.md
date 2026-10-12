# 0050: Game assets have no structure beyond the extracted file tree

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

The file system (1,799 files, 30 MB in `extract/files/`) is copied into the built ROM unchanged. Nothing describes what each file holds or which code loads it, and nothing turns the files into forms a person can edit and rebuild. Mods (plan M3) need to change maps, unit data, graphics and text, so they need that.

## Notes

- The plan lists "asset extraction into editable forms, kept outside git (D5)" under M2. This issue tracks it.
- Top-level directories: `battle`, `bmap`, `boot`, `combat`, `combat_st`, `commo`, `data`, `icon`, `info`, `mapedit`, `mu`, `national`, `ohashi`, `rikiishi`, `syogun`, `sys`, `xinfo`. `data/sound_data.sdat` holds the sound data. `boot/` holds the Download Play child (issue 0049), which is a program, not an asset.
- D5 (changed 2026-10-11): game data that we have reversed into a text form we edit enters git, the same as our C; raw asset files still do not. What git can hold: the formats we work out, the tools that unpack and repack them, the text forms, and a map from each file to the functions that load it.
- The campaign, Versus, War Room and Survival maps are not files: their headers, tiles and units are in ov000. They are in `assets/maps/` now (`docs/maps.md`, `tools/maps.py`). Combat mode's four maps are files, `/combat/%03x` (`CombatSetup_LoadMapPreviews`): LZ77 `MapData` with their own tile code table (`/combat/res_combat_pnl`). They are still copied.
- A repacked file must match the original byte for byte, the same as the code.

## Done when

Each file type has a documented format and a tool that unpacks it and repacks it to the same bytes. The build repacks the file system from the unpacked forms, and `make release` still matches. Split by file type when work starts.
