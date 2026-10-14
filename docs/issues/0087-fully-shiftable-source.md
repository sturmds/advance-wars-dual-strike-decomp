# 0087: Make the source fully shiftable

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-14
- **Milestone:** M3
- **GitHub:** #2
- **Related:** 0086 (shifted builds must run; the goal and hazard classes), 0012 (hard-coded addresses), 0054 (ARM7 words), 0064 (terrain stats framing), 0083 (aliases)

## Problem

Issue 0086 set the goal, decided by sturmds: no address, layout value or size written as a number, and no fixed struct size. Every size, copy length, stride, offset and bound comes from the structure (`sizeof`, fields, `offsetof`, element counts), so that growing a function, struct or table rebuilds a working ROM. The matching build cannot show where the source falls short: a number matches as well as a `sizeof` does.

This issue tracks that work by measure. `tools/shift_gaps.py` counts each kind of gap; `PROGRESS.md`, section "Shiftable", shows the counts after every commit. `shift_gaps.py --list KIND` names the sites.

## Measures (2026-10-14, mainline fbf58fd09)

| Kind | Count | What it is | Ends when |
|---|---:|---|---|
| `alias` | 331 | An alias in `aliases.txt` written as BASE+number | Each offset is computed from the C type (`offsetof`, `&x[k].f`) and checked; then the count is the aliases not yet checked that way. Target 0. |
| `scan_layout` | 6 | Words at a module or section bound with no relocation (`scan_pointers.py`) | Each is a linker symbol or listed with a reason in `shift_numbers.txt`. The 6 are BuildInfo and crt0 words that dsd rewrites or that the pilot fixed; the scanner should leave out what it has checked. Target 0. |
| `scan_high`, `scan_medium` | 0, 54 | Words that equal a symbol's start, with no relocation | Each is read: a relocation if a pointer, a number in `shift_numbers.txt` or a deleted dsd symbol if not. Target 0. |
| `scan_low` | 1,115 | Words in an address range, mostly 16-bit pairs | Falls as tables get their types (`mixed_u32`, below). No target of its own. |
| `alloc` | 11 | `Mem_Alloc` with a literal size | `sizeof` of a struct. The 0xc000 Download Play work area and the hardware-format buffers stay; each left gets a named size. Target 0 literals. |
| `copy` | 303 | Copy, fill and clear calls with a literal length | `sizeof` for struct and array copies (about 25 whole-struct copies first). VRAM, tile and palette sizes get named constants. Target 0 literals. |
| `offset` | 74 | `(u8 *)x + N` in place of a field | A field access. Target 0. |
| `loop` | 940 | `for` loops with a literal bound | A count from the array (`ARRAY_COUNT`, to be added to `types.h`) or a named constant (`CO_COUNT`, player and unit limits). Target 0. |
| `array_u32`, `array_char`, `array_other` | 6,388, 3,815, 974 | Initialised arrays whose length is a number (and again in `symbols.txt`) | `[]`, the length from the `.inc.c` initializer, where mwccarm emits the same bytes. Done by `data.py`, not by hand. Target 0. |
| `mixed_u32` | 887 | `u32` tables holding pointers and numbers | The table's struct type, so that a grown struct grows each row. Tool-driven (`data.py` and `ds.py`). Target 0. |
| `extern` | 515 | Extern arrays with a literal length | `[]`, or the length's named constant from the header that defines the table. Target 0. |
| `placeholder` | 279 | `u8 NAME[N]` standing for a zeroed variable (in `src/` and `bss.py gen` output) | The variable's own type, so that it grows with its struct. `bss.py gen` writes the declared type when a header gives one. Target 0. |

Growing measure: structs with a size assert (`typedef char X_size_check[...]`): 24. Every fixed format (save, link packets, map records) should have one.

Not counted by the tool, tracked in issue 0086: ARM7's module parameters and `WMSP_WORK` (issue 0054), the secure-area CRC (issue 0056), `.exceptix` and `.ctor` placement for new C++ code.

## Progress

- 2026-10-14, scanner review: `scan_layout` 6 to 0, `scan_high` 0, `scan_medium` 54 to 0, `scan_low` 1,115 to 218. No real pointer without a relocation turned up. `scan_pointers.py` leaves out the words `shift_numbers.txt` lists (20 added: the PXI command words of the two PM functions, three pool constants, BuildInfo's SDK version, ov002 fighter script bytes and `BattleSideWeapon` bit fields) and the .data and .rodata words that the C type of their object makes numbers (903: `PredeployedUnit`, `MapHeader`, `MapTileData` and u8, u16 and s16 tables). The 6 layout words were BuildInfo's .bss end (listed) and five words in u8 tables. The 218 left are all in `u32` tables that `data.py` wrote from the bytes for want of a declared type; they fall as those tables get their types.
- 2026-10-14, shift-data-lengths: `data.py` writes `[]` and `data.py open-counts` rewrote the C written before. `array_u32` 6,388 to 741, `array_char` 3,815 to 0, `array_other` 974 to 9: what is left is in ov001, for the ov001 agent to run `open-counts` on. `extern` 515 to 0: 396 became `[]`; 116 keep a count because mwccarm's code changes without it, and 3 are taken by `sizeof`; those use named constants (`QUAD_VERTEX_COORDS`, `DPAD_PATTERN_COUNT`, `INFO_PANEL_WIDTH_COUNT`, `LINK_SYNC_ARMY_COUNT`, `LINK_SYNC_CO_COUNT`, `SOUND_COMMAND_POOL_COUNT`) shared with the definition.
- 2026-10-14, ov001 scripts (issue 0066): every object in ov001's `.data` has its type (`MapScript`, `EventTrigger[]`, `EventScriptCommand[]`) and a `[]` length, from struct rows in `data_inc.txt`. `mixed_u32` 887 to 501 (ov001 386 to 0), `array_u32` 741 to 0 and `array_other` 9 to 0 (the ov001 tables left by shift-data-lengths), size asserts 24 to 27. ov001's C has none of the other kinds left (`alloc`, `copy`, `offset`, `loop`, `extern`, `placeholder`: 0).
- 2026-10-14, `alias` 331 -> 54 (shift-alias-check): `tools/alias_check.py` compiles every alias's FIELD with mwccarm and `make release` fails when it disagrees with the offset; 14 alias bases got the fields their aliases name. The 54 left are compiler bases (`+0x1000`), gMenuState's screen work areas at fixed offsets, flat `u8`/`u32`/`void*` tables standing for rows (their types belong to the data work), and the crt0 autoload list inside `data_02173788`.
- 2026-10-14, shift-ov001-names: the 156 ov001 scripts that map headers and campaign missions point to have names by map id (`Ov001_gMapScript_0e0`, issue 0066), in `src/game/ov001/map_scripts/`, and `maps.py pack` reads the old `data_ov001_` names of map files unpacked before. Names only: every count is unchanged (`mixed_u32` 501, `array_u32` 0).
- 2026-10-14, shift-sdk (src/sdk/, src/arm7/): `offset` 33 to 0, `copy` 60 to 0, `loop` 89 to 4, `alloc` 0, `placeholder` 34 to 25, `alias` 54 to 52; size asserts up by 5 (`RomHeader`, `IsdDoubleBytes`, `ExceptionRecord`, `WmspTxFrame`, `WlJoinBssDesc`). Lengths and bounds use `sizeof`, `offsetof`, `ARRAY_COUNT` or constants in the SDK headers (`MAC_ADDRESS_SIZE`, `SSID_SIZE`, `CHANNEL_LIST_SIZE`, `AID_COUNT`, `THREAD_ID_COUNT`, `SOUND_CHANNEL_COUNT`, `BITS_PER_BYTE` in types.h, ...). crt0's autoload list and main's .bss end in BuildInfo are linker symbols (`AUTOLOAD_LIST_START`, `AUTOLOAD_LIST_END`, `MAIN_BSS_END`). Left: four loops (a bit walk in `WifiParent_StoreMpReply`, `double2hex`'s `>= 1`, which changes the code as `> 0`, and two WL loops over 2 and 7 items whose meaning is not known) and 25 placeholders with no code names or split across files.

## How to count

- `python3 tools/shift_gaps.py` prints the table that `PROGRESS.md` shows; `--list KIND` prints `file:line: text` for each site.
- The source kinds read `src/` and `include/` with comments taken out, split into game code and SDK code. The scan rows and `mixed_u32` need a build (`make`); without one they show `-`.
- A pattern that counts the wrong thing is a bug in `shift_gaps.py`: fix it with a case in `tools/tests/test_shift_gaps.py`, and say in the commit that the count moved for that reason.

## Plan, in order

1. **Small, by hand:** `alloc` (11), `offset` (74), the whole-struct copies in `copy`. Check each changed function with `fn.py diff`.
2. **Aliases from C** (issue 0086 plan step 7): a check that compiles each alias's offset with mwccarm and compares it with `aliases.txt`.
3. **Scanner review:** read the 54 medium words and the 6 layout words; teach `scan_pointers.py` to leave out what `shift_numbers.txt` lists.
4. **Generated output:** `bss.py gen` and `data.py` write declared types and `[]` lengths. This moves `placeholder`, `array_*`, `extern` and `mixed_u32` by thousands, and needs no hand edits.
5. **Loops and the rest of `copy`:** add `ARRAY_COUNT` and named hardware sizes; take them file by file, as agent work in the decomp loop.

## Done when

Every count in the table is 0, or each site left is listed in a file that says why the number stays (for example, a hardware register layout), and `shift_gaps.py` leaves the listed sites out.
