# Text

The game's strings are data in overlay 0 (ov000), not files. `Text_Get(id)` (`src/game/ov000/text/text_get.c`) looks an id up in `gTextBanks`: 48 banks of `{firstId, strings}` (`TextBank`, `include/game/text_ref.h`), sorted by first id going down and ended by an entry whose table is 0. It takes the first bank whose first id is at or below the id and returns `strings[id & 0xffffff]`. The high byte of an id picks the bank (`0xf0000123` is string 0x123 of bank `0xf0000000`); a `TextRef` holds the same id as an index and a group (`0xf000`).

`tools/text.py` writes every bank into a YAML file and back. Like the maps, the sound archive and the graphics, the text is game data from your ROM and never enters git (tenet 9): `make setup` unpacks it into `extracted/AWRE/assets/text/`. No ROM file is built from these files yet (issue 0090).

```
tools/text.py unpack                       # every bank -> extracted/AWRE/assets/text/
tools/text.py get 0xf0000002               # one string by id (from the unpacked files; --rom reads the ROM)
tools/text.py grep "von bolt"              # ids and text of the strings a regular expression finds, any case
tools/text.py pack                         # every bank -> build/text/<id>.bin, the bytes as in ov000
tools/text.py pack --c --out build/text-c  # every bank -> C, plus gTextBanks.inc.c
tools/text.py check                        # every bank of the ROM through a bank file and back; compare bytes
```

## Files

```
extracted/AWRE/assets/text/
  banks.yaml       the bank files in gTextBanks order, and the id of the entry that ends the list
  f0000000.yaml    one bank, named by its first id
  c0000000.yaml
  ...
```

A bank file:

```yaml
first_id: 0xf0000000
table: data_ov000_022febc0          # the pointer table's symbol in ov000
address: 0x022f6d78                 # where the bank starts in ov000; only banks the ROM has
sha1: 0123...                       # of the ROM's bytes for the bank
strings:
  0x000: {bss: data_ov000_023503c4} # an empty string, a 4-byte object in ov000 .bss
  0x001: "Nell"
  0x01c: "All right! And I'll win\nnext time, too!"
  0x05a: {same: 0x012}              # the same string object as entry 0x12
  0x0f0: null                       # a null pointer (no bank has one)
order: [0x003, 0x00d, ..., table]   # the bank's objects in memory order
```

Each string is a YAML double-quoted string. The game's line break is written as a new line, and the codes are in braces (below). `get` and `grep` follow `same:`, and `grep` prints line breaks as `\n`.

## Encoding

One byte per character, ended by a 0 byte. Bytes 0x20-0x7e are ASCII and 0xa1-0xff Latin-1: the glyph widths in `Ov000_gGlyphWidths` (one byte per code) fit those letters, and the US text uses one Latin-1 letter, the é of "passé" (`0xf00002ef`). The text printer (`TextPrinter_ReadControlCode`, `TextPrinter_Update`) handles the codes below 0x20; the plain drawers (`Text_DrawStringToTiles`, `Text_GetStringWidth`) draw every byte as a glyph.

| Written | Bytes | What the text printer does |
|---|---|---|
| new line | 0x0d | Line break: the cursor goes down 2 tiles, back to the left edge |
| `{icon:N}` | 0x09, 0x80+N | Draws menu icon N (`PopupMenu_SetItemIcon`, N 0-51), 2 tiles wide. `Text_GetWidthWithCodes` counts 16 pixels |
| `{cargo:1}`, `{cargo:2}` | 0x0a, 0x11 or 0x12 | Draws the HP and icon of the acting unit's first or second cargo, 2 tiles wide |
| `{speed:N}` | 0x0b, 0x10+N | 0: shows the rest at once (`TextPrinter_ShowAll`); 1-9: N frames per character |
| `{clear}` | 0x0c | Clears the window and starts again at its top, without waiting |
| `{pause}` | 0x0e | A short pause (script `data_02158364` with 10), only while characters come one at a time |
| `{wait}` | 0x0f | Shows the dialogue cursor and waits (`DialogueCursor_Animate`, script `data_021584f8`), then starts again at the top of the window: the end of a page |
| `{color:N}` | 0x10+N, N 0-3 | Colour of the glyphs that follow (`TextPrinter.unk_32`) |
| `{0x14}`, `{0x16}`, `{0x17}` | 0x14, 0x16, 0x17 | Start script `data_02158520` and move the cursor 8 tiles right; 0x17 sets its `unk_1e` to 1. 0x16 and 0x17 end the yes/no questions (issue 0091) |
| `{space:N}` | 0x18+N, N 0-7 | A blank glyph N pixels wide. `Text_AddSpacerCodes` adds them to line names up with the tiles |
| `{0xNN}` | NN | Any other byte: 0x01-0x08, 0x15 (drawn as a glyph), `{` (0x7b), 0x7f-0xa0 and the soft hyphen 0xad |

A code whose parameter byte is out of its range is written as `{0xNN}` and its parameter as the character it is, so every string comes back to the same bytes. `encode` stops on an unknown code or a character outside the font.

## Layout in ov000

The strings and tables of all banks are one run of ov000 `.data`, 0x022f6d78-0x0234eb64, one bank after the other in the order of `gTextBanks`, with no other data between them. Each bank looks like one source file of the original: one `char` array per string and the pointer table, emitted by mwccarm smallest first (`tools/mwcc_order.py`, `docs/workflow.md`, "Several data objects in one file"), on 4-byte boundaries. Its empty strings are 4-byte objects in ov000 `.bss` (0x023503a4-0x023504d4), where the compiler puts objects that are all zero; their order there follows the banks too. A string object used by two entries of one bank is written once, as `same:`. No entry points into another bank.

The strings of one size are not in index order, and no simple definition order gives their order, so each bank file keeps `order`, the bank's objects as they sit in memory. `pack` places the objects by size, smallest first, and the ones of one size in the order of `order`; a string that is not in the list (a new one) goes after the listed ones of its size. An unchanged bank file gives back the ROM's bytes, which `check` and `tools/tests/test_text.py` test for every bank.

## pack: bytes and C

The ROM build does not read the bank files yet. It defines every string from the ROM's bytes: `config/AWRE/data_inc.txt` has one row per string object and table, and C files such as `src/game/ov000/ov000_data_022f6d78.c` include the values (`docs/workflow.md`, "Data from the ROM"). Building the text from the bank files, as `maps.py pack` builds the map tables, would mean taking those 5,000 rows and their C files out of the build and generating them; issue 0090 tracks it. Until then `pack` writes two forms:

- Bytes (the default): `build/text/<id>.bin` holds the bank as it sits in ov000 from its `address`, table words included, and `pack` says whether it equals the ROM. Use it to check an edit and to compare.
- C (`--c`): `<id>.c` defines one `char` array per string and the table (`const void* <table>[]`), in the order that makes mwccarm emit them as `pack` lays them out; `test_text.py` compiles three banks to check that. Empty strings refer to the existing `.bss` symbols. With no file named, `pack --c` also writes `gTextBanks.inc.c`, the values of `gTextBanks` from `banks.yaml`; for the ROM's banks it equals the file `data_inc.py` writes.

## Adding strings and banks

- A string at the end of a bank takes the next index: add `0x2fc: "..."` after the last entry of `f0000000.yaml`. Its id: the bank's first id plus the index (`0xf00002fc`). The table grows by a word.
- A new bank is a new file with `first_id`, `table` (the name to give its table, such as `gTextBank_f1000000`) and `strings`, and its name in `banks.yaml`. The list must keep its first ids going down, since `Text_Get` takes the first bank at or below the id: a bank for ids `0xf1000000` and up goes first, before `f0000000.yaml`. A bank the ROM does not have has no `address`, so `pack` writes it only as C (`--c`); the C and `gTextBanks.inc.c` are what a build that adds the bank compiles.
- Text_Get does not check the index against the table's length; an id past the last string reads whatever follows the table.

## Banks

What each bank holds, from its strings. `text.py get` and `grep` show the rest.

| First id | Strings | What |
|---|---|---|
| `0xf0000000` | 764 | CO text: names at 0x001-0x01b in `enum CoId` order (0x000 is empty), then win quotes, tag battle lines, power names (from about 0x17b), power quotes (about 0x1f3) and the profiles with their power descriptions (about 0x2bb) |
| `0xc0000000` | 772 | General: unit names, map names, menu items with icons, rules, help and yes/no questions |
| `0x80000000` | 57 | Main menu descriptions |
| `0x7f000000` | 49 | Versus and battle rule descriptions (fog, weather, CPU AI) |
| `0x7e000000` | 54 | Design Room (map editor): help and terrain names |
| `0x7d000000` | 37 | Screen titles in capitals (`WIRELESS`, `BATTLE MAPS`) |
| `0x7c000000` | 192 | Force Rank skill names (`text_ref.h`) |
| `0x7b000000` | 193 | Force Rank skill descriptions (`text_ref.h`) |
| `0x7a000000` | 456 | Records and medals |
| `0x79000000` | 15 | Labels of the records and Survival screens (total, turns, time) |
| `0x50000000` | 135 | Battle Maps shop and its unlock messages |
| `0x48000000` | 17 | Wireless errors and messages |
| `0x45000000` | 72 | Sound Room song titles |
| `0x44000000` | 31 | Combat mode setup |
| `0x43000000` | 8 | Combat mode help pages and items |
| `0x42000000` | 3 | Combat mode tutorial |
| `0x41000000` | 3 | Results lines (COs used, wallpaper unlocked) |
| `0x40000000` | 75 | Map dialogue outside the campaign story |
| `0x3e000000`-`0x23000000` | 3-212 each | Campaign dialogue, one bank per mission from the first (`0x3e`) down; 28 banks |
| `0x21000000` | 4 | Campaign narration (prologue and epilogue) |
| `0x00000000` | 1 | An empty string in `.bss`, for every id below `0x21000000` |

## Not covered yet

- The build does not read the bank files (issue 0090).
- The purpose of codes 0x14-0x17 and of the script `data_02158520` (issue 0091).
- Text outside the banks: strings that functions draw straight from their own data (`Text_Print` with a pointer), and the text in `res_*` files and pictures (`docs/gfx.md`).
