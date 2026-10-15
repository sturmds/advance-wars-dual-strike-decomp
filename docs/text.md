# Text

The game's strings are data in overlay 0 (ov000), not files. `Text_Get(id)` (`src/game/ov000/text/text_get.c`) looks an id up in `gTextBanks`: 48 banks of `{firstId, strings}` (`TextBank`, `include/game/text_ref.h`), sorted by first id going down and ended by an entry whose table is 0. It takes the first bank whose first id is at or below the id and returns `strings[id & 0xffffff]`. The high byte of an id picks the bank (`0xf0000123` is string 0x123 of bank `0xf0000000`); a `TextRef` holds the same id as an index and a group (`0xf000`).

`tools/text.py` writes every bank into a YAML file and back. Like the maps, the sound archive and the graphics, the text is game data from your ROM and never enters git (tenet 9): `make setup` unpacks it into `extracted/AWRE/assets/text/`, and `make` builds ov000's strings and tables from those files (below, "Build").

```
tools/text.py unpack                       # every bank -> extracted/AWRE/assets/text/
tools/text.py get 0xf0000002               # one string by id (from the unpacked files; --rom reads the ROM,
                                           #   --built the ov000 of the last build)
tools/text.py grep "von bolt"              # ids and text of the strings a regular expression finds, any case
tools/text.py pack                         # every bank -> build/text/<id>.bin, the bytes as in ov000
tools/text.py pack --c                     # every bank -> the C the build compiles, in build/gen/text/
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
table: gTextBank_f0000000           # the pointer table's symbol in ov000
address: 0x022f6d78                 # where the bank starts in ov000; only banks the ROM has
sha1: 0123...                       # of the ROM's bytes for the bank
strings:
  0x000: {bss: data_ov000_023503c4} # an empty string of the ROM, a 4-byte object in ov000 .bss
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

## Build

1. `tools/text.py pack --c` (the `TEXT` step) reads `banks.yaml` and the bank files and writes `build/gen/text/`: `bank_<id>.inc` for each bank (its strings as `char` arrays and its table as `const void* <table>[]`), and `text_banks.h` and `text_banks.inc`, the tables' declarations and the values of `gTextBanks`. It rewrites only the files that change, so an edit to one bank recompiles that bank alone.
2. `src/game/ov000/text/banks/text_bank_<id>.c`, one per bank of the ROM, includes its `bank_<id>.inc`; `src/game/ov000/text/text_banks.c` defines `gTextBanks` from the other two. `config/AWRE/arm9/overlays/ov000/delinks.txt` gives each bank file the bank's range in ov000 `.data`, in the order of `gTextBanks`. The empty strings of the ROM stay in `src/game/ov000/text/text_bss.c`.
3. Each bank's C defines its objects in the order that makes mwccarm emit them as `pack` lays them out (`tools/mwcc_order.py`), so an unchanged bank file gives back the ROM's bytes. `test_text.py` compiles three banks and checks every bank's file and `delinks.txt` range.

The C has the symbols that `symbols.txt` gives the ROM's objects, so objdiff compares the banks symbol by symbol: the table of bank `0xf0000000` is `gTextBank_f0000000`, and the string first used by its entry 0x1c is `gTextBank_f0000000_01c`. `symbols.txt` holds each string's exact size, without the pad bytes to the next 4-byte boundary. A bank file unpacked before a table got its name says `table: data_ov000_<address>`; `pack` takes the name `symbols.txt` gives that address, and `make setup` writes the new name into the files.

The step needs the unpacked files: without `make setup` the build stops on the missing `extracted/AWRE/assets/text/.unpacked`. `make setup` unpacks the text again and overwrites edits, as it does for the maps; `tools/wt_setup.sh` copies the main checkout's files into a worktree.

A string or bank that changes size moves everything after it in ov000. Such a build no longer matches retail, so it belongs in a mod build, not in a commit here; the shiftable build (issue 0086) lets the rest of the game follow the move. A text bank that grows moves the end of ov000 and with it the arena start that main holds in its secure area, so `fix_header.py` stops on such a build (issue 0056).

## Adding strings and banks

- A string at the end of a bank takes the next index: add `0x2fc: "..."` after the last entry of `f0000000.yaml` and run `make`. Its id: the bank's first id plus the index (`0xf00002fc`). The table grows by a word.
- A new bank is a new file with `first_id`, `table` (the name to give its table, such as `gTextBank_f1000000`) and `strings`, and its name in `banks.yaml`. The list must keep its first ids going down, since `Text_Get` takes the first bank at or below the id: a bank for ids `0xf1000000` and up goes first, before `f0000000.yaml`. A bank the ROM does not have has no `address` and no file in `src/game/ov000/text/banks/`: `pack --c` writes it into the `.inc` of the next bank in `banks.yaml` that has one (of the last, for a bank at the end), and `gTextBanks` grows by an entry. `pack` without `--c` skips it, since it has no place in the ROM's layout.
- An empty string written `""` points at the 0 byte that ends the bank's first string that is not empty: an array of its own would be all zero, and the compiler would put it in `.bss`, where the build places no bank objects. A bank whose strings are all empty writes them as `{bss: SYMBOL}`, an existing empty string such as `data_ov000_023504d0`.
- `same: N` must name an entry that holds a string that is not empty.
- Text_Get does not check the index against the table's length; an id past the last string reads whatever follows the table.
- To see the strings in the build: `tools/text.py get ID --built` or `grep PATTERN --built` read the ov000 that the last `make` linked, with the symbols from `build/arm9.elf.xMAP`.

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

- The purpose of codes 0x14-0x17 and of the script `data_02158520` (issue 0091).
- Text outside the banks: strings that functions draw straight from their own data (`Text_Print` with a pointer), and the text in `res_*` files and pictures (`docs/gfx.md`).
