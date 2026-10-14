# 0087: Make the source fully shiftable

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-14
- **Milestone:** M3
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
