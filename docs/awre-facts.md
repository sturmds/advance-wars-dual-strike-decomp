# AWRE facts

What we know about the US/AUS ROM, from plan step 0.2 (2026-10-06). Everything here was found offline with dsd 0.12.x, metroskrew 0.1.3 and binutils.

## ROM

- `ADVANCEWARSE`, game code `AWRE`, maker `01`, revision 0, 33,554,432 bytes.
- SHA-1 `5a8d22d41caa2c0ff41fb4a2b885a6be607e22d9`, MD5 `6119b65d4f854953a5b85bc692166476`. Both match TASVideos' "Good" entry for "Advance Wars - Dual Strike (USA, Australia).nds" (https://tasvideos.org/Games/2894/Versions/View/3068).
- Library version 2.0: build info `sdk_version` = `0x02007532`. The ROM has no library, Metrowerks or MSL banner strings and no build dates.
- ARM7 (157,268 B at 0x02380000) contains "2.39.00". Not analysed yet; probably the stock SDK ARM7 component.

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
  - The linked MSL/runtime code equals the CodeWarrior 1.2 base..sp2 library objects that metroskrew ships (42 functions, 11.6 KB).
- Flags that break matches: `-constpool`, `-O4,s`, any lower `-O`, dropping `-interworking`. `-ipa` does not exist in 2.0, so file boundaries do not affect inlining.
- The EU notes say "build 56", which is not in metroskrew. Nothing here needs it. To show 56 is required, you would need a compiled function that builds 72–82 cannot reproduce.
- The game code contains no Thumb. The four non-BIOS Thumb functions (matrix helpers at 0x0210969c, 0x021096bc, 0x021096d8 and an RL decompressor at 0x0210f9a8) are hand-written library assembly. They match as `asm void` under `#pragma thumb on` in a `.c` file.
- The linker is `mwldarm` 2.0-72. The zero-C link reproduces every module with it.

## dsd problems and workarounds

- `dsd init` (0.12.0 and 0.12.1) aborts: "Local function call from 0x021347a6 in ARM9 main to 0x021341aa leads to no function". Its function search runs past the end of `.text` into `.rodata` graphics data. `tools/dsd_init.py` works around it by zeroing 0x02129760–0x0212b860 in a temporary copy. That range held 29 real pointers, all in the MSL exception table at 0x02129768 (28 function entries plus one pointer to action data at 0x02129760). They were added to `relocs.txt` by hand on 2026-10-06, so the workaround leaves no known gap. The other pointer-like words in the range (`0x01ff8888`, `0x01ff9999`, `0x01ffffff`, …) fall outside ITCM's 0x01ff8000–0x01ffde80 or look like fill patterns, so they are data.
- 28 warnings about calls into the middle of functions, all in runtime and floating-point assembly. dsd adds labels for them.
- `dsd fix find-exceptix` reports "__FindExceptionTable not found".
- `dsd rom build` needs an ARM7 BIOS dump to compute the secure-area CRC. We don't use one: `tools/fix_header.py` checks that the secure area is unchanged, copies the original CRC, and recomputes the header CRC.

## Hard-coded addresses noticed

- None recorded in our code yet. See the `.rodata` relocation gap above.
