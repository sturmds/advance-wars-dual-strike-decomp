# 0086: A shifted or modded build must still run

- **Status:** in progress
- **Area:** build
- **Opened:** 2026-10-14
- **Milestone:** M3
- **Related:** 0006 (emulator check), 0012 (hard-coded addresses), 0019 and 0055 (overlay relocations), 0023 (mod build repo), 0050 (Download Play child), 0054 (ARM7 words), 0056 (secure-area CRC), 0060 (symbol sizes), 0064 (terrain stats framing), 0083 (aliases)

## Problem

A person who changes code or data almost anywhere (grows a function, adds a table row, enlarges an overlay) and rebuilds should get a ROM that still works, while the unmodified build still matches retail byte for byte. The matching build cannot show where this fails: a number that should have been an address matches as well as a symbol does.

It failed for real. A mod grew main and ov000 by 0x40 bytes each, and melonDS stopped on boot with `ARM9: data abort (022BB318)` in `Ov004_CheckTimedEvent`. `OS_GetInitArenaLo` returned the number 0x0237a5a0 (the end of the overlays in retail) as the start of the main arena. The overlays then ended 0x80 bytes past the heap's start, and loading one overwrote the heap's first blocks. `make shift` (main padded, symbol moves checked) and `make emu-smoke` (DeSmuME for 20 s) both passed that ROM.

This issue holds the scope: the kinds of hazard, how many of each, the plan in order, and how each step is tested.

## Pilot (2026-10-14)

- **The arena fix.** `OS_GetInitArenaLo` returns linker symbols for the main, ITCM and DTCM arenas: `SDK_MAIN_ARENA_LO`, `SDK_SECTION_ARENA_ITCM_START` and `SDK_SECTION_ARENA_DTCM_START`.
  - `tools/configure.py` lists them in `ARM9_LAYOUT_SYMBOLS`, each with its linker-script value and its retail value. `fn.py diff` and `check` compare against the retail value.
  - `tools/lcf_absolute.py` writes them at the end of `build/arm9.lcf`. The main arena is a section placed in dsd's `SPACE` region (`ORIGIN = AFTER(ARM9, OV000, ..., OV006)`), whose start, rounded to 32 bytes, is the first address past main and every overlay. The other two are `ITCM_BSS_END` and `DTCM_BSS_END`.
  - `SDK_SECTION_ARENA_DTCM_START` was a fixed number (0x027c12e0) in `ARM9_ABSOLUTE_SYMBOLS`. `OS_InitThread` writes a stack guard word at that address and `OS_GetInitArenaHi` reads it, so growing DTCM would have put the guard on a variable.
  - `ADDR(SPACE)` (dsd's `__CODE_HI`) links as 0 with mwldarm 2.0-72, because no section is placed in `SPACE`. `__ITCM_HI = ADDR(ITCM) + SIZEOF(ITCM)` links as 0x01ff8000. No code uses either.
  - Without the fix, the retail ROM still matches.
- **The shift test.** `tools/shift_build.py` takes `--module` (main, itcm, dtcm, ovNNN; repeatable) and runs three checks:
  - `symbols`: every symbol moves by the pads before it, or stays.
  - `arena`: the main arena starts past every module.
  - `stale`: no word that the link left unchanged holds the old address of a symbol that moved, apart from the numbers in `config/AWRE/shift_numbers.txt`.

  `make shift` runs main, ov000, itcm, dtcm, and main with ov000 at 0x40, the crash case.
- **Failing, then passing.** The crash case, `shift_build.py --module main --module ov000 --pad 0x40`:
  - With the old `OS_GetInitArenaLo` it exits 1: `stale: 1 words kept the old address of a moved symbol`, `ARM9 0x0210e354 (OS_GetInitArenaLo+0xd0) = 0x0237a5a0, the old address of OV001_DATA_END`.
  - With the fix it exits 0, and the arena starts at 0x0237a620, the new end of ov001.
  - The ITCM and DTCM shifts fail the same way on the old code (`0x01ffde80`, `0x027c12e0` in the same pool) and pass with the fix.
  - The stale check found the problem; the arena check did not need to.
- **What the stale check cannot see.** It sees only a word whose value equals a symbol's start. Two kinds of pointer get past it: one to the middle of an object, and one built at run time.

## Hazard classes and counts (2026-10-14, mainline 054c0bb65)

### 1. Address constants with no relocation

Two sources of hits:

- `tools/scan_pointers.py`, ARM9: 1 high, 57 medium, 1,129 low. On 10-11 it was 5/27/1,049. The data work deleted 114 false relocations; all 114 words are hits now, 38 medium and 76 low.
- A boundary scan written for this issue. It looks for values at or up to 0x20 past any section or module end, and for literal-pool words in RAM outside every module. It found 21 hits.

Result:

- **Real pointers with no relocation: 0.** A cross-check read all 32,923 ABS32 relocations of the linked objects. Every `kind:load` relocation in `relocs.txt` is in the link, so the data moved into C kept its pointers. The C adds 11 relocations that `relocs.txt` lacks: the SDK constants and `Isd_RestoreFiq`/`Isd_RestoreIrq` + 8.
- **Layout constants: 6.** All six are fixed or handled now:
  - the three `OS_GetInitArenaLo` words: fixed above;
  - `SDK_SECTION_ARENA_DTCM_START` in `OS_InitThread` and `OS_GetInitArenaHi`: fixed above;
  - `BuildInfo[4]` (main's .bss end at 0x02000b10): `dsd rom build` writes the moved value into the ROM (checked in the shifted ROM).
- **Fixed hardware or SDK addresses: about 15.** Examples: the 0x023fe800 boot area, 0x027c3000, the protection-region words, the 0x02000000 exception stack. They are right as numbers.
- **Numbers: the rest.** Byte, halfword and pixel tables, map units (`PredeployedUnit` is all `u8`), and ov002 fighter scripts. The medium count is inflated: dsd left symbols at the targets of the deleted false relocations (`data_021f0200`, `data_ov000_022e022d`, ...), and a value that equals a symbol start ranks medium.
- **ARM7** (issue 0054):
  - `WMSP_WORK` (0x027d6c44, the start of ARM7 .bss, in 6 pool words that decompiled C reads) and `Entry_Arm7`'s 0x02380170 have no relocation.
  - `BuildInfo_Arm7` and the crt0 words are numbers that `tools/arm7.py bin` does not rewrite.
  - ARM9 shifts never move ARM7, so these matter only when ARM7 changes.

### 2. Layout facts the code assumes

| Where | Value | Status |
|---|---|---|
| `OS_GetInitArenaLo` main, ITCM, DTCM | 0x0237a5a0, 0x01ffde80, 0x027c12e0 | fixed (pilot) |
| `SDK_SECTION_ARENA_DTCM_START` (`OS_InitThread`, `OS_GetInitArenaHi`) | 0x027c12e0 | fixed (pilot) |
| main `BuildInfo` (autoload list, .bss bounds), autoload entries, overlay table | | `dsd rom config` / `dsd rom build` rewrite them (checked with grown dtcm, itcm and ov001) |
| `data_02175180`, `data_02175198` (aliases inside `data_02173788`) | crt0's autoload list | The list lands inside the task pool by chance; dsd writes the right address into the ROM, but the symbols mislead. Replace them with linker symbols or numbers. |
| `MAIN_ARENA_HI` 0x023c0000, stacks at the top of DTCM, Download Play limits | | hardware; never move |
| ARM7 `BuildInfo_Arm7`, `data_arm7_02380160`, arena lows on `data_arm7_027d84fc` / `0380afec` | | not rewritten when ARM7 grows (issue 0054) |

Growth budgets, which nothing checks:

- **Game heap:** 0x023c0000 − 0x0237a5a0 = 0x45a60 bytes (279 KiB, 64 KiB of it the small heap). Every byte added to main or to the largest overlay group comes out of it.
- **ITCM:** 0x2180 bytes left.
- **DTCM:** data growth comes out of the system stack, about 0x28a0 bytes.

### 3. Sizes written as numbers

These do not move anything, but a mod that grows a struct overruns memory without an error.

- **`Mem_Alloc`:** 118 calls, 30 of them with a literal size (`ov005_screen_alloc_large_work.c` 0x97e0, `func_ov005_02362618.c` 0x2f4, `func_020d4288.c` 0xa14), plus `#define`d work sizes (`OV005_SOUND_ROOM_WORK_SIZE`, `NAME_ENTRY_WORK_SIZE`). These are the riskiest.
- **Copy, fill and clear calls:** 574, 303 of them with a literal size. In game code, 83 are not VRAM, tile or palette sizes. About 25 copy a whole struct and should use `sizeof`: the 0x724 design-map record, the 24-byte link packets, `player_info_init.c`'s 26-byte names, and others.
- **Byte offsets in place of fields:** 34 sites of `(u8*)x + 0xNN` in 25 files.
- **Loop bounds:** 818 `for` loops have a literal bound. There is no `ARRAY_COUNT`.
- **Size asserts:** 2 exist (`TerrainStats`, `UnitStats`).
- **`bss.py gen`:** 107 of its 114 definitions are `u8 NAME[0x...]` sized from `symbols.txt`. When the header that declares the variable's type grows, the variable does not.
- **What does not pin anything:** the sizes in `symbols.txt`. The linker never reads them, so a table that grows in C moves everything after it, and its relocations follow.

### 4. Aliases

`config/AWRE/aliases.txt` has 310 aliases. All are `real`, and every one is in the same module and section as its base. This pass checked against the C types the 141 aliases that issue 0083 had not covered, and found no hidden variable.

Aliases are fragile by nature. An alias is `BASE + constant`, so it stays right when code or data before its base grows, and when a field is appended after it. It goes wrong in three cases:

- a field is inserted before it;
- the element size changes and the alias sits on element 1 or later (`gCoTable+0x4e8`, `gMenuButtonReserved`, the ov002 cell layouts, ov003's +0xd00/+0xe00);
- it is a compiler base, and its assembly user keeps a fixed offset.

The lasting fix is to compute each offset from the C (`offsetof`, `&x[k].f`, compiled with mwccarm the way `header_merge.py` checks layouts). Also:

- `Ov000_gTerrainStats` is framed 5 words into a flat `u32[192]` (issue 0064).
- `maps.py lcf` writes `data_ov000_022df026` and `data_ov000_022e022d` inside `Ov000_gMapHeaders`. They have no user.

### 5. What the build fixes on its own, and what nobody fixes

Fixed by dsd or our tools when a module changes size:

- main `BuildInfo` and the autoload list;
- the overlay table, including the ctor bounds;
- FAT, FNT, banner, ARM7 offset and ROM size;
- header sizes and entry points;
- the header CRC;
- ARM7's autoload table (`arm7.py bin`).

Main and the overlays are not compressed, so there is nothing to recompress.

Nobody fixes:

- the secure-area CRC (issue 0056);
- ARM7's module parameters (above);
- the overlay relocations that name several overlays (issue 0055). The stale check reports them in ov001-ov006 shifts, for example `ARM9 0x0216a6ec = 0x02350560` and the ten ov000 words at `0x022e5288`-`0x022e5c88`;
- `.exceptix` and `.ctor` for new C++ code: the link drops our objects' `.exceptix`, and `dsd lcf` places no new `.ctor` section;
- `__CODE_HI` and `__ITCM_HI`, which have wrong values (unused);
- the Download Play child `mb_combat_sgn.srl`, which is copied unchanged (issue 0050).

### 6. Tests

- `make shift` (above). It is not part of `make release`: it needs `arm-none-eabi-readelf`, which the build does not, and it takes about 40 s. Putting it into release needs an ELF reader in Python (`fnindex.read_elf` is most of one) and sturmds's decision on the time.
- Emulator: melonDS 1.1 (a local build at `~/repos/melonDS/build/melonDS`, with its GDB stub) replaces DeSmuME. Issue 0006 has the boot check; it must stop on the crash ROM above and pass the fixed one.

## Plan, in order

1. **Done: the arena pilot and the shift checks.**
2. **Leftover symbols and scanner.** Delete the dsd symbols that only existed as targets of deleted false relocations. That shrinks `shift_numbers.txt` and the medium count. Scanner changes: take relocations from the linked objects, add a level for section-end values, scan pools across all of RAM with an allowlist of hardware addresses, give holder and file per hit, scan ARM7 too. Size: small; in progress on branch `shift-symbols`.
3. **Issue 0055: the 15 relocations that name several overlays.** They make every ov001-ov006 shift fail the stale check, so they are the next real bug class. Follow the callers to `FS_LoadOverlay`, or watch in melonDS. Size: medium. Then add the ov001-ov006 shifts to `make shift`.
4. **The emulator boot check on melonDS** (issue 0006). Size: medium; in progress on branch `melonds-check`.
5. **Sizes:** the 30 literal `Mem_Alloc` sizes and the `*_WORK_SIZE` defines become `sizeof`; then the ~25 whole-struct copies; then size asserts on the save and link formats. Matching is unchanged, since `sizeof` is a constant. Check each with `fn.py diff`. Size: medium, mechanical.
6. **Growth budgets in the shift test:** report the heap left (arena hi − arena lo), the ITCM room and the DTCM stack in every shifted build, and fail below a floor. Size: small.
   - 2026-10-14: done. Each shifted build prints the room left in the game heap, ITCM and the DTCM stack, and fails when one is used up (a dtcm pad of 0x3000 leaves the stack -0x760 bytes and fails; the link itself does not complain). No floor above zero: how much heap the game needs is only known from playing.
7. **Aliases from C:** compute each alias offset with `offsetof` from the C type, and fail when it disagrees with `aliases.txt`. Replace `data_02175180`/`data_02175198` with linker symbols. Delete the two unused map-header symbols. Size: medium.
8. **`bss.py gen` types:** use the declared type when a header gives one, not `u8[size]`. Size: small to medium.
9. **ARM7** (issue 0054): `arm7.py bin` rewrites `BuildInfo_Arm7` and the crt0 words from ELF symbols; `WMSP_WORK` becomes a `.bss` variable. Size: medium. It matters only when ARM7 changes.
10. **New C++ code:** place `.exceptix` and new `.ctor` sections in the script. Size: small, if a mod ever needs it.
11. **The secure-area CRC** (issue 0056) and a ROM larger than 32 MiB (the capacity byte at 0x14). These are for hardware targets.
12. **Byte-offset macros to fields, `ARRAY_COUNT` for loops over C tables.** Ongoing, file by file.

## Done when

`make shift` grows every module (ov001-ov006 included) and passes all three checks; the melonDS check boots each shifted ROM to the title screen without an abort; and the mod repo (issue 0023) builds a playable ROM with a change that grows main.
