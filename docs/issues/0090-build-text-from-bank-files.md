# 0090: The build does not take the text from the bank files

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-15
- **Milestone:** later
- **Related:** 0050 (asset structure), 0082 (data linking), 0086 (shiftable ROM)

## Problem

`tools/text.py unpack` writes every text bank of ov000 into `extracted/AWRE/assets/text/`, and `pack` turns the files back into the ROM's bytes or into C (`docs/text.md`). The build reads neither: an edited bank file does not change `build/AWRE.nds`. A mod that changes a string or adds a bank has to write the C by hand.

## Notes

The build defines the text from the ROM's bytes today:

- `config/AWRE/data_inc.txt` has a row for each string object and table in ov000 0x022f6d78-0x0234eb64 (5,023 rows), and C files such as `src/game/ov000/ov000_data_022f6d78.c` define each object from its `.inc.c` file. Some 4-byte strings are typed `u32` there.
- `gTextBanks` (`src/game/ov000/ov000_data_022f6bd8.c`) takes its values from `data_inc.py`, with a fixed size of 0x188 bytes.
- The empty strings are size-1 symbols in ov000 `.bss` (0x023503a4-0x023504d4), written by `bss.py gen` for `ov000_bss_02350320.c`.

To build the text from the bank files, as `maps.py pack` builds the map tables:

- A build step runs `text.py pack --c` and the C it writes replaces the text's data_inc rows and C files, one file per bank. `pack --c` already writes the definitions in the order that makes mwccarm emit the ROM's layout (`tools/tests/test_text.py` compiles three banks), and the `gTextBanks` values that equal `data_inc.py`'s.
- The `.bss` empty strings stay where they are, or move into the bank files as zero-initialized arrays; the second needs the `.bss` order checked against `bss.py`'s runs.
- `delinks.txt` gets one entry per bank over its range, and `symbols.txt` keeps exact sizes; a string whose length changes must still link in a shifted build (issue 0086).
- `make setup` already runs `text.py unpack`; the build would then need it before ov000 compiles, as it needs the map files.

## Done when

`make` builds ov000's strings and tables from the bank files, `make release` still matches retail, and an edited string or a new bank reaches `build/AWRE.nds` with `make mod`.
