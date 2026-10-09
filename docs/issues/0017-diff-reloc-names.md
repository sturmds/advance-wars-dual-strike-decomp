# 0017: `fn.py diff` shows less than 100% for table accesses that match

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-08
- **Milestone:** M1

## Problem

dsd gives a symbol to every address code points at, including addresses inside a table (`data_ov000_022f4fd4` = `Ov000_gUnitStats + 0x1c`). C that reads the field through the table makes a relocation to `Ov000_gUnitStats+0x1c`. The bytes are the same and `make release` passes, but objdiff compares symbol names and reports 99.4–99.9%. In batch 3, seven functions matched this way; agents had to reason that the difference was only the name.

## Notes

- `fn.py diff` could resolve both sides of each relocation to an address before comparing, and count equal addresses as a match.
- Or remove the inner symbols from `symbols.txt` once a table has a struct type, so dsd names the relocation after the table.

- 2026-10-08: fixed in `fn.py diff`. When objdiff marks a row as differing only in a relocation, both sides are looked up in `symbols.txt` and compared as address plus addend. Equal addresses are marked `=` and count as matching. 29 complete functions went from 99.x% to 100%; C with a wrong field offset still fails.

## Done when

`fn.py diff` reports 100% for these functions.
