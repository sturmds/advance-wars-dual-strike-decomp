# 0012: Hard-coded addresses would break a shifted build

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-07
- **Milestone:** M3

## Problem

A matching build can contain addresses written as numbers instead of symbols. They still match, but once code or data moves (modding), they point at the wrong place.

## Notes

- Known gap: none in our code yet. The `.rodata` range blanked for `dsd init` had 29 real pointers, now restored (issue 0007).
- Data tables are the likely problem: pointers inside `.data`/`.rodata` that dsd did not find.
- Plan M3 test: build with padding inserted in one module, then run the emulator scenario (issue 0006).
- The tenets review flags `0x02xxxxxx` literals in C.

- 2026-10-11: shift test, static part. `make shift` builds `build/shift/AWRE.nds` with 0x20 bytes inserted in main before `window_wipe_start_hide_current.o` (0x02006a20). It links, `dsd rom build` accepts the larger main, and 23,712 ELF symbols moved by 0x20 while 491 (itcm, dtcm, main before the pad, constants) stayed; none did anything else. A 0x1000-byte pad at 0x02082cf4 gives the same result. Every word that differs between the two builds' modules sits at a relocation. `dsd rom build` updates main's module parameters (.bss start and end, autoload list at 0x02000b00) and the overlay table on its own. `make emu-smoke` boots both shifted ROMs the same as retail as far as that test can see (issue 0006).
- Overlay relocations: issue 0019 (closed) and 0055 (15 left).

### Scanner results (2026-10-11)

`tools/scan_pointers.py` (see `docs/workflow.md`) checks every aligned word in .data, .rodata and the non-instruction parts of .text, in all ARM9 modules, against the address ranges of main, itcm, dtcm and the overlays, and skips words that have a relocation.

| Confidence | Count | What they are |
|---|---|---|
| high | 5 | 4 real pointers, now in `relocs.txt`: crt0's two loads of its module parameters (`0x020008ec`, `0x02000a0c` → `BuildInfo` at `0x02000b00`), and itcm's pointers to `Screen_HBlankTopPanel` and `Screen_HBlankBottomPanel` (`0x01ff8f64`, `0x01ff8f68`). The fifth, `0x02000b10` = `0x022ad560`, is main's .bss end in the module parameters, which `dsd rom build` rewrites. |
| medium | 27 | Read one by one: all numbers. Values point into the middle of functions or are byte patterns (`0x02040104`, `0x02111000` eight times in an ov002 table, `0x02000000` = start of main RAM in SDK code). |
| low | 1,049 | Not reviewed. Most are byte tables: `0x02010004` alone is 252 hits, `0x02010000` 70, `0x02020202` 64. 675 are in ov000 .data, 216 in main .data and .rodata, 119 in ov002 .data. |

The four pointers found point at crt0 and itcm, which the default shift does not move, so they did not show up in the shift test. The low list may still hide some, mostly pointers to unaligned data (strings) with no symbol at the target. Check a low hit when the code that reads it is decompiled, or when a shifted build misbehaves near it.

The scanner only finds words whose value is an address. It cannot find pointers built in code from two halves, offsets from one known address to another added at run time, or numbers that dsd wrongly turned into relocations (issue 0055 has 10 suspects in ov000).

### Shiftable-ROM pass (2026-10-14)

The arena crash in `OS_GetInitArenaLo` (a layout address written as a number, past the end of the overlays, so the scanner's in-module test missed it) started issue 0086, which now holds the plan. Counts at mainline 054c0bb65: high 1, medium 57, low 1,129. All were classified: no real pointer lacks a relocation; 6 are layout constants (now linker symbols, or rewritten by `dsd rom build`); the rest are numbers or fixed hardware addresses. The rise since 10-11 is the 114 false relocations deleted by the data work. `make shift` now also finds words that kept the old address of a moved symbol (issue 0086).

### Scanner results (2026-10-14)

The scanner now also counts the relocations of the linked objects, adds a `layout` level (a value within 0x20 bytes after a module or section boundary or the end of the overlays, tested first), checks literal pools against all of ITCM, main RAM and DTCM, leaves out 9 fixed hardware and SDK addresses, and names the holder and file of each word.

| Level | Count | What they are |
|---|---|---|
| layout | 6 | crt0's module parameters hold main's .bss end (`dsd rom build` rewrites it); 5 byte-table words `0x02000001`-`0x0200001c` that only match main's start, which does not move. Before the arena fix in 7d07c56fa it also found `OS_GetInitArenaLo`'s three bounds, the end of the overlays among them. |
| high | 0 | |
| medium | 54 | Was 57. 32 are byte tables whose value equals a placeholder object's address (below). |
| low | 1,115 | Was 1,129. |

11 words have a relocation only in the linked objects: the absolute symbols in `OS_InitThread`, `OS_GetInitArenaHi` and `OS_GetInitArenaLo`, and the pointers in the pools of `Isd_RestoreFiq` and `Isd_RestoreIrq`, which `relocs.txt` lacks.

`scan_pointers.py --unused-symbols` lists the data symbols nothing refers to. Two were aliases inside `Ov000_gMapHeaders` (`data_ov000_022df026`, `data_ov000_022e022d`) left by false relocations from the sine table and an LZ77 stream; they are gone. 78 start an object defined in C and stay. Of those, 32 lost their last relocation in the commits that deleted false relocations, so dsd made them from numbers: merging each with the object before it would end the false matches above, but changes the C layout.

## Done when

A build with padding inserted in main runs the M3 scenario the same as retail.
