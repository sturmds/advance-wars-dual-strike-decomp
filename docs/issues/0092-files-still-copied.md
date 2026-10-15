# 0092: 596 files of the ROM's file system are still copied as they are

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-15
- **Milestone:** later
- **Related:** 0049 (Download Play child), 0050 (asset structure, closed), 0023 (mod build)

## Problem

The build takes the file system from `build/rom/files/` (`tools/rom_files.py`). Build steps write 1,203 of the 1,799 files there: the 1,202 picture files (`docs/gfx.md`, "Build") and the sound archive (`docs/sound.md`, "Build"). The other 596 are links to `extract/files/`, so a mod can change them only by hand in `extract/files/`, which `make extract` overwrites.

By top-level directory: `battle` 216, `ohashi` 97, `mu` 78, `bmap` 70, `rikiishi` 67, `combat` 21, `icon` 13, `commo` 6, `mapedit` 6, `combat_st` 5, `info` 5, `syogun` 4, `boot` 3, `sys` 3, `xinfo` 2.

## Notes

- The `res_*` containers: one LZ77 stream holding several parts of a screen (tiles, maps, palettes, cells). `gfx.py check` leaves them out (`docs/gfx.md`, "Not covered yet").
- Palettes and other files that are not compressed: read, not written back.
- Combat mode's four maps, `/combat/%03x` (`CombatSetup_LoadMapPreviews`): LZ77 `MapData` with their own tile code table (`/combat/res_combat_pnl`).
- `boot/`: the Download Play child `mb_combat_sgn.srl` (issue 0049), a program, not an asset.
- A file type joins as the pictures and the sound archive did: a tool that unpacks it into `extracted/` and packs it back to the same bytes, a build step that writes into `build/rom/files/` with `restat`, and its paths in `build/rom/built.txt`, which `configure.py` writes.

## Done when

Every file type has a documented format and a tool that unpacks and repacks it, the build packs it, and `make release` still matches. Split by file type when work starts.
