# 0054: 25 ARM7 words that look like pointers have no relocation

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

`tools/arm7_init.py` ends by listing the words it guessed were pointers into ARM7 but could not give a relocation. They keep their original bytes, so the build matches, but C that reads them as symbols will not match until they have one. 25 are left:

- **Section ends and `.bss` starts.** `0x027d6c44` (5 words, in main RAM code) and `0x03807888` / `0x0380788c` / `0x03807890` / `0x03807894` (7 words, in WRAM code) point at the end of an autoload's data, or into the first 0x1c bytes after it. In the original each autoload's `.bss` starts right there (the crt0 clears from the end of the data); dsd's `.bss` sections start 32-byte aligned (0x027d6c60, 0x038078a0), so those addresses lie in no section. `0x027d6c58` (1 word) is the same case.
- **Constants.** `0x03800001`, `0x03800002`, `0x03800004`, `0x03800010`, `0x03800020`, `0x03800080`, `0x03800406`, `0x03800902`, `0x03800b01` (12 words) point into the middle of WRAM functions, so they are flags or bit masks, not pointers. These are correct as they are.

## Notes

- The first group looks like linker-made values (the end of an autoload, a heap or arena start). ARM9 has the same kind of value as an absolute linker symbol (`ARM9_ABSOLUTE_SYMBOLS` in `tools/configure.py`, `tools/lcf_absolute.py`). ARM7 could get its own list, passed to the same script after `arm7.py lcf`, once a function that reads one is decompiled and its name is known.
- Starting each ARM7 `.bss` section at the unaligned address would put the 0x1c-byte variables in a section too, but dsd's link script aligns `.bss` to 32 bytes, so that needs a change to the generated script (`arm7.py lcf`).

## Done when

The section-end words are relocations to named symbols, and the matching ARM7 C uses them.
