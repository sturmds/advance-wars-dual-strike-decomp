# 0092: 563 files of the ROM's file system are still copied as they are

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-15
- **Milestone:** later
- **Related:** 0049 (Download Play child), 0050 (asset structure, closed), 0023 (mod build)

## Problem

The build takes the file system from `build/rom/files/` (`tools/rom_files.py`). Build steps write 1,236 of the 1,799 files there: the 1,202 picture files and the 33 wallpapers (`docs/gfx.md`, "Build", "res_* files") and the sound archive (`docs/sound.md`, "Build"). The other 563 are links to `extract/files/`, so a mod can change them only by hand in `extract/files/`, which `make extract` overwrites.

By top-level directory (before the wallpapers, 33 of the `ohashi` files): `battle` 216, `ohashi` 97, `mu` 78, `bmap` 70, `rikiishi` 67, `combat` 21, `icon` 13, `commo` 6, `mapedit` 6, `combat_st` 5, `info` 5, `syogun` 4, `boot` 3, `sys` 3, `xinfo` 2.

## Notes

- The `res_*` containers: parts one after the other (LZ77 tiles and maps, raw palettes and cells), with no header; the code of each screen knows where each part starts (`docs/gfx.md`, "res_* files"). `gfx.py` reads the 33 wallpapers. Next with the same shape: `tagbreak_black`, `tagbreak_mix`, `tagbreak_union`, `titlecall_wall1`/`2`, `totalrank_wall1`-`3`, `loading1`/`2`; each needs its loader read for the map size and the part starts, and a repacked file needs those starts in the code to follow it (as `build/gen/gfx/wallpaper_offsets.h` does for the wallpapers) or its parts kept in place (as for `res_wall_base`).
- Palettes and other files that are not compressed: read, not written back.
- Combat mode's four maps, `/combat/%03x` (`CombatSetup_LoadMapPreviews`): LZ77 `MapData` with their own tile code table (`/combat/res_combat_pnl`).
- `boot/`: the Download Play child `mb_combat_sgn.srl` (issue 0049), a program, not an asset.
- A file type joins as the pictures and the sound archive did: a tool that unpacks it into `extracted/` and packs it back to the same bytes, a build step that writes into `build/rom/files/` with `restat`, and its paths in `build/rom/built.txt`, which `configure.py` writes.

## Done when

Every file type has a documented format and a tool that unpacks and repacks it, the build packs it, and `make release` still matches. Split by file type when work starts.
