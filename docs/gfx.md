# Graphics

The game's pictures are 4-bit tiles: 8x8 pixels, 32 bytes, two pixels a byte with the left pixel in the low nibble. Most files under `extract/files/` hold them LZ77-compressed (type 0x10, the format the BIOS unpacks). Palettes are 16 BGR555 colours (`0bbbbbgggggrrrrr`, colour 0 transparent), and the ones for CO pictures are in main's data, not in files. The ROM has no Nitro graphics files (NCGR, NCLR, NCER, NANR, NSCR) and no NARC archives: every file is raw tiles, an LZ77 stream, or a `res_*` container (below).

`tools/gfx.py` turns tile data into indexed PNG files and back. Like the maps and the sound archive, the PNG files are game data from your ROM and never enter git (tenet 9): `make setup` unpacks every picture file of the ROM into `extracted/AWRE/assets/gfx/files/`, and `make` packs them back into the ROM's file system (below, "Build").

```
tools/gfx.py unpack                              # every picture file -> extracted/AWRE/assets/gfx/files/
tools/gfx.py co VON_BOLT                         # every picture of one CO, with palette 0 (a view; the build does not read it)
tools/gfx.py unpack extract/files/syogun/076 --palette main:0x021329b8
tools/gfx.py pack extracted/AWRE/assets/gfx/co/11_VON_BOLT/face_v0_m0.yaml --out build/076
tools/gfx.py check                               # every /syogun file: unpack, pack, compare bytes
tools/gfx.py layouts
```

`unpack FILE` writes `NAME.png` and `NAME.yaml`; a file of `extract/files/` goes under its ROM path in `extracted/AWRE/assets/gfx/files/`, the files the build packs. The YAML file names the source, the compression, the bits per pixel, the layout, the size, the palette used for the PNG and the SHA-1 of the original bytes. `pack` reads the PNG beside it (any indexed PNG: 1, 2, 4 or 8 bits, any filter, not interlaced) and writes the bytes, compressed again when the source was; it says whether they match the ROM. `--palette-out FILE` also writes the PNG's palette as BGR555.

## LZ77

A 4-byte header (0x10 and the decompressed size in 3 bytes), then groups of a flag byte and 8 items, high bit first: a clear bit is a literal byte, a set bit a 2-byte reference (4 bits of length - 3, 12 bits of distance - 1). The game's files come from a compressor that takes the longest match, the nearest one among equals, never uses distance 1 (so an unpack to VRAM, which takes 16-bit writes, reads only bytes already written) and pads to 4 bytes with zeros. `lz77_pack` does the same, and gives back every LZ77 file in the ROM byte for byte (1,229 files in `syogun`, `battle`, `bmap`, `rikiishi`, `xinfo`, `ohashi` and others; `gfx.py check --dir`).

## Layouts

A layout lists the sprite pieces (OBJ shapes) a picture is drawn with. The tiles fill the pieces in order, each piece row by row (1D mapping). `grid:N` is one piece N tiles wide. Without `--layout`, `unpack` picks one by size.

| Layout | Size | Pixels | Pieces | From |
|---|---|---|---|---|
| `co_full_top` | 0x3600 | 144x192 | six 64x64 in rows of two, six 16x32 down the right | `gCoPortraitSprite` |
| `co_full_bottom` | 0x2400 | 144x128 | four 64x64, four 16x32 | the legs, 192 pixels below the top (`TagBreak_DrawSidePiece`) |
| `co_face` | 0x480 | 48x48 | rows of 32x8 and 16x8 | `data_02151df8` |
| `co_battle` | 0x3c0 | 40x48 | grid 5 wide | battle screen BG |
| `co_medium` | 0x240 | 24x48 | 16x32, 8x32, 16x16, 8x16 | `gCoPictureSprite` |
| `co_small` | 0x180 | 48x16 | 32x16, 16x16 | `data_02151ccc` |
| `co_hud` | 0x100 | 32x16 | 32x16, drawn mirrored | `data_ov000_0234ebf0` |

## CO pictures

`gCoGraphics` (main, 33 rows of 21 words) holds them, by CO id. Rows 28 to 32 are not COs but one soldier of each army (Orange Star, Blue Moon, Green Earth, Yellow Comet, Black Hole) for dialogue: they have only a face (`0bd` to `0c1`, the same file for each mood) and use the blank pictures of row 0. `enum CoSoldier` (`include/game/co_graphics.h`) numbers them from `CO_COUNT`, and the data_inc row `gCoGraphics_soldiers` writes them to a file of their own, so rows for new COs go between. Event scripts keep the retail numbers 28 to 32 (`EVENT_SOLDIER_FIRST`); the three picture commands turn them into rows with `EVENT_PICTURE_TO_CO_MOOD`. Picture variant 0 or 1 (`Co_GetPictureVariant`) picks the CO's normal or alternate outfit; the palette variant (0-7, or `CO_CLONE_PALETTE` for a clone) picks one of 8 palettes.

| Words | What | Where | Loaders |
|---|---|---|---|
| 0, 2 | Full picture, top (variants 0, 1) | `/syogun/000`-`01a`, `036`-`050` | `Co_LoadGraphics`, `Co_LoadArmyGraphics` (CO select, CO power and tag cut-ins, results) |
| 1, 3 | Full picture, legs | `/syogun/01b`-`035`, `051`-`06b` | `Co_LoadObjPicture` (tag cut-ins only) |
| 4-6, 7-9 | Face, 3 moods per variant | `/syogun/06c`-`0bc`, `0c2`-`112` | `Co_LoadLargePicture`, `Co_LoadArmyLargePicture` (dialogue, Combat setup, win and lose screens) |
| 10-15 | Battle face, 3 moods per variant | `/syogun/149`-`1ef` | `Co_LoadMoodPicture` (battle screen) |
| 16, 17 | Small picture | main `data_021298b8`, `data_0212c138`, 0x180 bytes per CO | `Co_LoadSmallPicture`, `Co_LoadTexture` (menus, lists) |
| 18, 19 | Medium picture | `/syogun/113`-`148` | `Co_QueueArmyMediumPicture` (Versus slots) |
| 20 | 8 palettes | main `data_02131fb8`, 0x100 bytes per CO | `Co_CopyPalette` |

Outside the table: the map panel face (`CoPanel_Draw`, `PowerPanel_*Draw`) at main `gCoPanelFaces` and `gCoPanelFacesAlt` (variants 0 and 1), 0x100 bytes per CO from CO 1 (`CO_PANEL_FACE_ROW`); and 12 compressed tiles in each `gCoTable` row (`Co_UnpackPicture`: results, CO rank, Combat mode menus), 5 pointers that all name the same stream. Those tiles look like the CO's name in letters, not a picture; the layout is not known yet.

The files are numbered by kind, then by CO id, so a new CO cannot take the next number in each run without renumbering the files after it; adding files at the end of `/syogun` and pointing a new `gCoGraphics` row at them avoids that (below, "Adding a file").

## res_* files

A `res_*` file (136 of them, in `ohashi`, `combat`, `combat_st`, `commo`, `icon`, `battle` and `sys`) has no header: it holds its parts one after the other, each an LZ77 stream padded to 4 bytes or raw bytes (palettes, cell data), and the code that loads it knows where each part starts. `gfx.py` reads the 33 wallpapers, `ohashi/res_wall_*`; the others are left as they are.

A wallpaper is one background layer:

| Part | What |
|---|---|
| tiles | LZ77, 8x8 tiles; the map can use at most 1,024 |
| map | LZ77, 16-bit entries: tile number in bits 0-9, horizontal and vertical flip in bits 10 and 11, palette in bits 12-15. In screen blocks of 32x32 entries: a 64x32 map is its left block, then its right one |
| palette | raw BGR555 colours, at the end of the file |

| Files | Tiles | Map | Palette | Loaders | Part starts |
|---|---|---|---|---|---|
| the 31 that `data_021595cc` names (wallpaper id 0, the CO ids, then `agb1`, `agb2`, `rankmax`) | 8-bit, 0xf000 bytes (960 slots) | 64x32 (512x256 pixels; the screen shows 320x192) | 256 colours, into extended palette slot 3 | `Wallpaper_LoadOnLayer2`, `Wallpaper_LoadOnLayer3` (the Super cut-in, `SuperBreak_SetUpScreen`), `Wallpaper_LoadBothOnLayer2`/`3` (the wallpaper list, the results), `Wallpaper_LoadSelected` | tiles at 0 (`data_0229c684`, zero), map and palette from `data_021593fc` and `data_021593bc`; the intro (`Ov004Task_InitScene`, `Ov004Task_RunSceneTimeline`) loads 8 of them with the starts in its code |
| `res_wall_base` | 4-bit, two sets of 0x2000 bytes | two 64x32 maps, one per screen | 16 colours, bank 15 | `Wallpaper_LoadSelected` | in the code: 0, 0x984, 0xd70, 0x169c, 0x1a90 |
| `res_wall_single` | 4-bit, 0x6000 bytes | 32x64 (the task scrolls it down) | 16 colours, bank 15 | `Wallpaper_LoadSingle`, `Wallpaper_LoadBoth` | in the code: 0, 0x2eac, 0x36ec |

The 4-bit maps use palette 0 in the file; the loaders add 0xf000 to every entry.

`unpack` writes one PNG per file with the whole map, as the screen would show it at scroll 0 (512x256 for a wallpaper; `res_wall_base` has its two maps one above the other, 512x512), and the file's own palette in the PNG. Unlike the CO pictures, the palette is part of the file, so a colour changed in the PNG's palette changes the game's. The YAML file has `format: res`, the parts (`compression`, `size` unpacked), the pictures (which parts hold the tiles and map, and their place in the PNG), the palette part, and `offsets` for a file whose starts the code holds.

`pack` cuts the PNG into 8x8 cells and builds tiles and map:

- When `extract/` has the file the YAML file came from (same SHA-1), it starts from its tiles and map. A cell that looks the same keeps its entry; a changed cell takes a tile that already looks like it (flipped or not), or a new tile in the first slot after those the original map uses. An unchanged PNG gives back the ROM's bytes.
- Without the original (a file a mod adds), or when the new tiles do not fit, every cell is matched from scratch in map order, sharing tiles that are equal or flipped copies. It stops when the picture needs more different tiles than the file has slots.

A wallpaper that packs to a different size moves its map and palette. The build writes `build/gen/gfx/wallpaper_offsets.h` from the files it puts in `/ohashi` (`gfx.py wallpapers`), with `RES_WALL_<NAME>_MAP` and `RES_WALL_<NAME>_PALETTE` for every `res_wall_*` file but `base` and `single`. `data_021593fc`, `data_021593bc` (`src/game/main/graphics/graphics_data_0215915c.c`) and the two intro functions use those names, so main and ov004 follow the packed file. For `res_wall_base` and `res_wall_single` `pack` keeps every part at its start and pads with zeros; it stops when a part would run past the next start.

`gfx.py check --dir extract/files/ohashi` unpacks and packs each wallpaper, and also packs it from scratch and checks that the result draws the same picture.

Tested by hand: a checkerboard painted over Jake's wallpaper (`res_wall_john`, in a colour put into an unused palette slot) shows behind him in his Super cut-in in melonDS, and the intro, which shows that wallpaper too, still plays. Before the intro took its starts from the header, it crashed on the repacked file.

To add a wallpaper, for example for a new CO: write `files/ohashi/res_wall_NAME.png` and `.yaml` (copy a wallpaper's YAML file and leave out `source` and `sha1`), add a string `"res_wall_NAME"` and a row in `data_021595cc`, `data_021593fc` (`RES_WALL_NAME_MAP`) and `data_021593bc` (`RES_WALL_NAME_PALETTE`) at its wallpaper id, and run `make`. The first `CO_COUNT` rows go by CO id, then the `WALLPAPER_EXTRA_COUNT` extras.

## Build

1. `make setup` runs `gfx.py unpack` with no file. It writes every picture file of the ROM under its ROM path: `extract/files/syogun/076` becomes `extracted/AWRE/assets/gfx/files/syogun/076.png` and `076.yaml`. Picture files are the 33 wallpapers (above) and the LZ77 streams of whole 4-bit tiles outside the `res_*` containers: 1,202 files in `syogun` (all 508 LZ77 files), `battle`, `bmap`, `rikiishi`, `xinfo`, `mapedit`, `national`, `info` and `sys`. `/syogun` files get the CO layouts and the CO's palette 0 from the `gCoGraphics` row that names them; the rest a grid and greys. Unpack writes only files that change and `.unpacked` last. As for the maps and text, `make setup` overwrites edits to the ROM's pictures; a file the ROM does not have stays.
2. `configure.py` gives each YAML file under `extracted/AWRE/assets/gfx/files/` a `GFX` step, `gfx.py pack YAML --out build/rom/files/PATH`, with the PNG beside it as an input. The step writes only a changed file (ninja `restat`), so an edited PNG repacks that file alone and a PNG saved with no change stops there.
3. The `GFX WALLPAPERS` step writes `build/gen/gfx/wallpaper_offsets.h` from the wallpaper files of `/ohashi`, packed or the ROM's (above), and the C files that include it are compiled again when it changes.
4. `tools/rom_files.py tree` (the `ROM FILES` step) fills the rest of `build/rom/files/` with links to `extract/files/`, and removes what neither `extract/files/` nor a `GFX` step has. `rom_files.py romconfig` points `files_dir` of `build/build/rom_config.yaml` at that directory, so `dsd rom build` takes the file system from there. The `ROM BUILD` step runs `tree` again first, which puts back a link removed by hand, and stops when a file a `GFX` step writes is missing, which happens once when a YAML file comes back with an old date (`mv`, `cp -p`): run `make` again.
5. An unchanged PNG gives back the ROM's bytes (`test_gfx.py` checks all 1,235 files), so `make release` still matches retail. A YAML file that is removed sends its path back to the ROM's file.

Without the unpacked files (a worktree whose main checkout has not unpacked them, for example) the build takes the ROM's files as they are. `tools/wt_setup.sh` copies the main checkout's `extracted/AWRE/assets/gfx/files/` when it holds a whole unpack.

A picture that packs to a different size moves the files after it in the ROM; the FAT gives their new places, and nothing in the code holds them. The pictures in main's data (small pictures, map panel faces) and the palettes are not files and are not rebuilt yet.

To replace a picture: edit the PNG in an editor that keeps the indexed palette (GIMP, Aseprite), using the 16 colours of the PNG's palette, and run `make`. Only the colour indices go into the file; the colours on screen come from a palette elsewhere (for the CO pictures, in main's data). The size must stay the same: `pack` stops on a PNG of other dimensions than the YAML file gives.

## Adding a file

The game opens these files by path: `Co_LoadLargePicture` calls `FS_ChangeDir("/syogun")`, then `File_UnpackTo(dest, name)` with the file name from `gCoGraphics`, and `FS_OpenFile` finds the name in the File Name Table at run time. No game code holds a file id. The functions that open files by id (`FS_OpenFileFast`, `FS_OpenFileDirect`) are called by the overlay loader, whose ids 0 to 6 come before every file, the sound archive, which gets its id from `FS_ConvertPathToFileID` at run time, and the SDK's archive set-up and Download Play code. So a new file costs no code change beyond what names it.

To add `/syogun/200`: write `extracted/AWRE/assets/gfx/files/syogun/200.png` and `200.yaml`, for example by copying the files of a picture of the same kind and leaving out `sha1` and `source`, and run `make`. The YAML file needs `compression` (`lz77` or `none`), `bpp`, `layout`, `width` and `height`; `size` defaults to width x height x bpp / 8. `configure.py` finds the new pair, and the file lands in the ROM's `/syogun`. A new directory works the same way (`files/mod/face.png` becomes `/mod/face`). The game reaches the file once something names it, such as a new `gCoGraphics` row with the string `"200"`.

What moves when a file is added:

- File ids. `dsd rom build` numbers the files in File Name Table order: each directory's files by name (case ignored), then its subdirectories by name, depth first, from 7 (after the overlays). A file added to `/syogun` takes the id after the last `/syogun` file it sorts behind, and every file after it, in `/syogun`, `/sys` and `/xinfo`, moves up by one. Nothing in the game depends on those ids (above).
- Places in the ROM. `dsd rom build` writes the files' data in the order of `path_order.txt`, which `dsd rom extract` takes from the ROM, and leaves out a file that no line covers: its FAT entry would stay 0-0. `rom_files.py` writes `build/rom/path_order.txt` with a last line `/`, which places every file not placed yet at the end. A file in a directory the list names (such as `/syogun`) is placed with that directory; a new directory goes at the end.

Tested by hand: a recoloured Jake picture (`/syogun/126` and the others of his row) shows on the CO select screen in melonDS, and a ROM with `/syogun/200` and `/zmod/face` added boots to the main menu with the ids of `/sys` and `/xinfo` moved up by one.

## Not covered yet

- The `res_*` files other than the wallpapers. Several have the same shape (`tagbreak_black`, `tagbreak_mix`, `tagbreak_union`, `titlecall_wall1` and `2`, `totalrank_wall1` to `3`, `loading1` and `2`), but where each part starts and what its map looks like is in the code of each screen, not checked yet.
- Palettes, the pictures in main's data and the files that are not compressed are read, not written back (the wallpapers' palettes excepted).
- A file that is neither a picture nor the sound archive (`docs/sound.md`, "Build"), such as a palette or one of the other `res_*` containers, can only be replaced by hand in `extract/files/`, which `make extract` overwrites (issue 0092).
