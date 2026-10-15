# 0050: Game assets have no structure beyond the extracted file tree

- **Status:** closed
- **Area:** build
- **Opened:** 2026-10-11
- **Milestone:** M2
- **Closed:** 2026-10-15. The build takes the ROM's file system from `build/rom/files/` (`tools/rom_files.py`) and packs the pictures and the sound archive into it from `extracted/`; the maps and the text are built into ov000. The 596 files still copied as they are moved to issue 0092, split by file type as "Done when" asks.

## Problem

The file system (1,799 files, 30 MB in `extract/files/`) is copied into the built ROM unchanged. Nothing describes what each file holds or which code loads it, and nothing turns the files into forms a person can edit and rebuild. Mods (plan M3) need to change maps, unit data, graphics and text, so they need that.

## Notes

- The plan lists "asset extraction into editable forms, kept outside git (D5)" under M2. This issue tracks it.
- Top-level directories: `battle`, `bmap`, `boot`, `combat`, `combat_st`, `commo`, `data`, `icon`, `info`, `mapedit`, `mu`, `national`, `ohashi`, `rikiishi`, `syogun`, `sys`, `xinfo`. `data/sound_data.sdat` holds the sound data. `boot/` holds the Download Play child (issue 0049), which is a program, not an asset.
- D5 (changed 2026-10-11): game data that we have reversed into a text form we edit enters git, the same as our C; raw asset files still do not. What git can hold: the formats we work out, the tools that unpack and repack them, the text forms, and a map from each file to the functions that load it.
- The campaign, Versus, War Room and Survival maps are not files: their headers, tiles and units are in ov000. They are unpacked into `extracted/AWRE/assets/maps/` by `make setup` now (`docs/maps.md`, `tools/maps.py`). Combat mode's four maps are files, `/combat/%03x` (`CombatSetup_LoadMapPreviews`): LZ77 `MapData` with their own tile code table (`/combat/res_combat_pnl`). They are still copied.
- A repacked file must match the original byte for byte, the same as the code.
- 2026-10-15: the build takes the file system from `build/rom/files/` (`tools/rom_files.py`): links to `extract/files/`, except the files a build step writes there. The 1,202 picture files are the first such files (`tools/gfx.py`, `docs/gfx.md`, "Build"); a new file under `extracted/AWRE/assets/gfx/files/` adds a file to the ROM. Another file type joins by writing its files into `build/rom/files/` and listing them for `rom_files.py tree`, as `configure.py` does for the pictures. Still copied as they are: the sound archive, the `res_*` containers, palettes and the other files that are not compressed.
- 2026-10-15: the sound archive joins the tree. A `SOUND` step runs `tools/sound.py pack` into `build/rom/files/data/sound_data.sdat` from `extracted/AWRE/assets/sound/`, so an edited or added sequence, bank or wave archive changes `build/AWRE.nds` (`docs/sound.md`, "Build" and "Adding a song"). What is left goes to issue 0092.

## Done when

Each file type has a documented format and a tool that unpacks it and repacks it to the same bytes. The build repacks the file system from the unpacked forms, and `make release` still matches. Split by file type when work starts.
