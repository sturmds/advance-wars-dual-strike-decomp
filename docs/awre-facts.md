# AWRE facts

What we know about the US/AUS ROM, from plan step 0.2 (2026-10-06). Everything here was found offline with dsd 0.12.x, metroskrew 0.1.3 and binutils.

## ROM

- `ADVANCEWARSE`, game code `AWRE`, maker `01`, revision 0, 33,554,432 bytes.
- SHA-1 `5a8d22d41caa2c0ff41fb4a2b885a6be607e22d9`, MD5 `6119b65d4f854953a5b85bc692166476`. Both match TASVideos' "Good" entry for "Advance Wars - Dual Strike (USA, Australia).nds" (https://tasvideos.org/Games/2894/Versions/View/3068).
- Library version 2.0: build info `sdk_version` = `0x02007532`. The ROM has no library, Metrowerks or MSL banner strings and no build dates.
- ARM7 (157,268 B at 0x02380000) contains "2.39.00". See "ARM7" below.

## Modules

| Module | Address | Size | ARM functions | Thumb functions |
|---|---|---|---|---|
| main | 0x02000000 | 1,499,168 | 5,925 | 22 |
| ITCM | 0x01ff8000 | 24,192 | 113 | 0 |
| DTCM | 0x027c0000 | 4,832 | data only | |
| ov000 | 0x022ad560 | 667,072 (25 KB code) | 121 | 0 |
| ov001 | 0x02350560 | 172,096 | 76 | 0 |
| ov002 | 0x02350560 | 112,224 | 164 | 0 |
| ov003 | 0x02350560 | 87,456 | 235 | 0 |
| ov004 | 0x022ad560 | 72,672 | 236 | 0 |
| ov005 | 0x02350560 | 112,160 | 270 | 0 |
| ov006 | 0x02350560 | 119,008 | 321 | 0 |

- 7 ARM9 overlays, no ARM7 overlays, 1,799 files in the file system.
- 7,461 ARM and 22 Thumb functions, about 1.69 MB of code. Counts are from dsd's analysis, so they can still shift as boundaries get fixed.
- Main's sections: `.text` to 0x02129760, `.rodata` to 0x02151c68, `.data` to 0x0216e020, `.bss` to 0x022ad560.
- Rough regions in main: game code 0x02000b24 to about 0x02109000; system library code about 0x02109000 to 0x0211e980; MSL C, runtime and floating-point library 0x0211e980 to 0x02129760.
- Exception table (28 entries) at 0x02129768, covering only the MSL/runtime region.

## Compiler

- **mwccarm 2.0, builds 72, 73, 79 or 82** (CodeWarrior for DS 1.2 base to sp2p3), flags `-O4,p -proc arm946e -interworking`. These four builds produced identical code in every test, so the build uses 2.0-72.
- Evidence:
  - 15 game functions from main, ov003, ov006 and ITCM match byte for byte (relocations masked). Nine of them match only builds 72–82.
  - 160 system library functions match exactly with 2.0-72. 2.0-84 matches 47 and 3.0-139 matches 25, all within the 160. So the library code uses the same compiler and flags as the game.
  - Across the ROM there are 8,809 `ldmfd {..,lr}; bx lr` epilogues and no `pop {..,pc}`. 2.0-84 and later emit `pop {pc}`.
  - The linked MSL/runtime code equals the CodeWarrior 1.2 base..sp2 library objects that metroskrew ships (42 functions, 11.6 KB). Some MSL functions do not match with 2.0-72 (by-value struct frame): see `docs/issues/0061-msl-by-value-struct-frame.md`.
- Flags that break matches: `-constpool`, `-O4,s`, any lower `-O`, dropping `-interworking`. `-ipa` does not exist in 2.0, so file boundaries do not affect inlining.
- The EU notes say "build 56", which is not in metroskrew. Nothing here needs it. To show 56 is required, you would need a compiled function that builds 72–82 cannot reproduce.
- The game code contains no Thumb. The four non-BIOS Thumb functions (matrix helpers at 0x0210969c, 0x021096bc, 0x021096d8 and an RL decompressor at 0x0210f9a8) are hand-written library assembly. They match as `asm void` under `#pragma thumb on` in a `.c` file.
- The linker is `mwldarm` 2.0-72. The zero-C link reproduces every module with it.

## ARM7

Found 2026-10-11 (issues 0013 and 0042). ARM7 is the second CPU: sound, touch screen, wireless, real-time clock and power.

| Part | Address | Size | `.bss` | ARM functions | Thumb functions | Code bytes |
|---|---|---|---|---|---|---|
| static (crt0) | 0x02380000 | 368 | 0 | 3 | 0 | 344 |
| main RAM autoload | 0x027c0000 | 93,252 | 6,328 | 533 | 0 | 91,504 |
| WRAM autoload | 0x037f8000 | 63,624 | 14,180 | 453 | 25 | 62,448 |

- `arm7.bin` is the static part, then the two autoloads, then a 24-byte autoload table (address, size, `.bss` size per autoload). The crt0 literal pool at 0x02380158 holds the module parameters. Entry and load address 0x02380000, no ARM7 overlays.
- 1,014 functions, 989 ARM and 25 Thumb, about 154 KB of code (dsd's analysis plus the fixes in `tools/arm7_init.py`). The 25 Thumb functions are the BIOS call stubs at 0x03803b74-0x03803bf0 (`svc N; bx lr`).
- WRAM holds `.text` to 0x0380713c, then read-only tables, then six more functions to 0x0380773c, then `.data`. The config keeps the tables inside `.text`.
- In the original each autoload's `.bss` starts right after its data (the crt0 clears from there), not 32-byte aligned as in dsd's sections. Sizes and the bytes of `arm7.bin` are the same either way; a few pointers into the first 0x1c bytes of `.bss` have no relocation (`arm7_init.py` lists them).
- **Game code: none found.** No word in ARM7 points into ARM9 memory (the 7 words in 0x02000000-0x0237ffff are constants such as `0x020000ff`), and the only strings are system library ones (`Failed sound alarm OS_SendMessage`, the version `2.39.00`). ARM7 counts as system library code. Comparing it with the ARM7 of another game built on library version 2.x would show whether that holds; the Download Play child's ARM7 is a different build (issue 0049).
- **Compiler: mwccarm 2.0 (builds 72-87 give the same code), `-O4,s -proc arm7tdmi -interworking`.** The same compiler as ARM9, optimised for size. Evidence (2026-10-11, C written from the disassembly, not committed): `func_arm7_027c0000` (a loop over 32 entries), `func_arm7_037f8000` and `func_arm7_038076e8` match only with 2.0 and size optimisation (`,s`): `-O4,p`, `-O3,p`, 3.0-114 to 3.0-139 and 4.0-1051 all differ. `-O1` to `-O3` with `,s` give the same code as `-O4,s` for these three, so the level is a guess until a function shows a difference (`-O0` differs). `func_arm7_027c0070` matches with every version. Dropping `-interworking` or using `-proc arm946e` breaks the matches. Functions show the 2.0 shape: `stmfd sp!, {lr}; sub sp, sp, #4` and `ldmfd sp!, {lr}; bx lr`.

## dsd problems and workarounds

- `dsd init` (0.12.0 and 0.12.1) aborts: "Local function call from 0x021347a6 in ARM9 main to 0x021341aa leads to no function". Its function search runs past the end of `.text` into `.rodata` graphics data. `tools/dsd_init.py` works around it by zeroing 0x02129760–0x0212b860 in a temporary copy. That range held 29 real pointers, all in the MSL exception table at 0x02129768 (28 function entries plus one pointer to action data at 0x02129760). They were added to `relocs.txt` by hand on 2026-10-06, so the workaround leaves no known gap. The other pointer-like words in the range (`0x01ff8888`, `0x01ff9999`, `0x01ffffff`, …) fall outside ITCM's 0x01ff8000–0x01ffde80 or look like fill patterns, so they are data.
- 28 warnings about calls into the middle of functions, all in runtime and floating-point assembly. dsd adds labels for them.
- `dsd fix find-exceptix` reports "__FindExceptionTable not found".
- `dsd rom build` needs an ARM7 BIOS dump to compute the secure-area CRC. We don't use one: `tools/fix_header.py` checks that the secure area is unchanged, copies the original CRC, and recomputes the header CRC.

## Hard-coded addresses noticed

- None recorded in our code yet. See the `.rodata` relocation gap above.
