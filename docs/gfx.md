# Graphics

The game's pictures are 4-bit tiles: 8x8 pixels, 32 bytes, two pixels a byte with the left pixel in the low nibble. Most files under `extract/files/` hold them LZ77-compressed (type 0x10, the format the BIOS unpacks). Palettes are 16 BGR555 colours (`0bbbbbgggggrrrrr`, colour 0 transparent), and the ones for CO pictures are in main's data, not in files. The ROM has no Nitro graphics files (NCGR, NCLR, NCER, NANR, NSCR) and no NARC archives: every file is raw tiles, an LZ77 stream, or a `res_*` container (below).

`tools/gfx.py` turns tile data into indexed PNG files and back. Like the maps and the sound archive, the PNG files are game data from your ROM and never enter git (tenet 9); write them under `extracted/AWRE/assets/gfx/` (the default) or `build/`. No ROM file is built from them yet.

```
tools/gfx.py co VON_BOLT                         # every picture of one CO, with palette 0
tools/gfx.py unpack extract/files/syogun/076 --palette main:0x021329b8
tools/gfx.py pack extracted/AWRE/assets/gfx/co/11_VON_BOLT/face_v0_m0.yaml --out build/076
tools/gfx.py check                               # every /syogun file: unpack, pack, compare bytes
tools/gfx.py layouts
```

`unpack` writes `NAME.png` and `NAME.yaml`. The YAML file names the source, the compression, the bits per pixel, the layout, the size, the palette used for the PNG and the SHA-1 of the original bytes. `pack` reads the PNG beside it (any indexed PNG: 1, 2, 4 or 8 bits, any filter, not interlaced) and writes the bytes, compressed again when the source was; it says whether they match the ROM. `--palette-out FILE` also writes the PNG's palette as BGR555.

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

`gCoGraphics` (main, 33 rows of 21 words) holds them, by CO id. Rows 28 to 32 are not COs but one soldier of each army (Orange Star, Blue Moon, Green Earth, Yellow Comet, Black Hole) for dialogue: they have only a face (`0bd` to `0c1`, the same file for each mood) and use the blank pictures of row 0. Picture variant 0 or 1 (`Co_GetPictureVariant`) picks the CO's normal or alternate outfit; the palette variant (0-7, or `CO_CLONE_PALETTE` for a clone) picks one of 8 palettes.

| Words | What | Where | Loaders |
|---|---|---|---|
| 0, 2 | Full picture, top (variants 0, 1) | `/syogun/000`-`01a`, `036`-`050` | `Co_LoadGraphics`, `Co_LoadArmyGraphics` (CO select, CO power and tag cut-ins, results) |
| 1, 3 | Full picture, legs | `/syogun/01b`-`035`, `051`-`06b` | `Co_LoadObjPicture` (tag cut-ins only) |
| 4-6, 7-9 | Face, 3 moods per variant | `/syogun/06c`-`0bc`, `0c2`-`112` | `Co_LoadLargePicture`, `Co_LoadArmyLargePicture` (dialogue, Combat setup, win and lose screens) |
| 10-15 | Battle face, 3 moods per variant | `/syogun/149`-`1ef` | `Co_LoadMoodPicture` (battle screen) |
| 16, 17 | Small picture | main `data_021298b8`, `data_0212c138`, 0x180 bytes per CO | `Co_LoadSmallPicture`, `Co_LoadTexture` (menus, lists) |
| 18, 19 | Medium picture | `/syogun/113`-`148` | `Co_QueueArmyMediumPicture` (Versus slots) |
| 20 | 8 palettes | main `data_02131fb8`, 0x100 bytes per CO | `Co_CopyPalette` |

Outside the table: the map panel face (`CoPanel_Draw`, `PowerPanel_*Draw`) at main `data_0212e9b8` and `data_021304b8` (variants 0 and 1), 0x100 bytes per CO from CO 1; and 12 compressed tiles in each `gCoTable` row (`Co_UnpackPicture`: results, CO rank, Combat mode menus), 5 pointers that all name the same stream. Those tiles look like the CO's name in letters, not a picture; the layout is not known yet.

The files are numbered by kind, then by CO id, so a new CO cannot take the next number in each run without renumbering the files after it; adding files at the end of `/syogun` and pointing a new `gCoGraphics` row at them avoids that.

## Not covered yet

- The `res_*` files (`ohashi/`, `sys/`, `combat/` and others): one LZ77 stream holding several parts of a screen (tiles, maps, palettes, cells). `check` leaves them out.
- Packed bytes do not go into the ROM: the build still copies `extract/files/`.
- Palettes and the pictures in main's data are read, not written back.
