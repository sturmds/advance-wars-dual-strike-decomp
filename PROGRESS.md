# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`. `game`, `SDK/library` and `all`
add both CPUs.

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| ARM9 game | 6700 / 6700 | 1553840 / 1553840 | 100.00% | 0 | 0 |
| ARM9 SDK/library | 657 / 833 | 114512 / 135230 | 84.68% | 176 | 20718 |
| ARM9 all | 7357 / 7533 | 1668352 / 1689070 | 98.77% | 176 | 20718 |
| ARM7 SDK/library | 951 / 1020 | 149340 / 154300 | 96.79% | 69 | 4960 |
| ARM7 all | 951 / 1020 | 149340 / 154300 | 96.79% | 69 | 4960 |
| game | 6700 / 6700 | 1553840 / 1553840 | 100.00% | 0 | 0 |
| SDK/library | 1608 / 1853 | 263852 / 289530 | 91.13% | 245 | 25678 |
| all | 8308 / 8553 | 1817692 / 1843370 | 98.61% | 245 | 25678 |

## By module

Rows with nothing done are left out, but count in the totals; ARM7's row is always shown.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 5180 / 5180 | 1082632 / 1082632 | 100.00% | 0 |
| main | SDK/library | 650 / 822 | 113600 / 133694 | 84.97% | 172 |
| itcm | game | 102 / 102 | 21896 / 21896 | 100.00% | 0 |
| itcm | SDK/library | 7 / 11 | 912 / 1536 | 59.38% | 4 |
| ov000 | game | 120 / 120 | 25300 / 25300 | 100.00% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 164 / 164 | 64208 / 64208 | 100.00% | 0 |
| ov003 | game | 235 / 235 | 82696 / 82696 | 100.00% | 0 |
| ov004 | game | 236 / 236 | 67364 / 67364 | 100.00% | 0 |
| ov005 | game | 266 / 266 | 96684 / 96684 | 100.00% | 0 |
| ov006 | game | 321 / 321 | 105588 / 105588 | 100.00% | 0 |
| arm7 | SDK/library | 951 / 1020 | 149340 / 154300 | 96.79% | 69 |

Modules with nothing done: none.

## decomp.dev report

From `build/artifacts/0.1_report/report.json`, which `make release` writes (`tools/decomp_dev_report.py`).
decomp.dev shows "Decompiled" (`matched_code`) and "Fully linked" (`complete_code`).
Functions written as `asm` count only when `config/AWRE/asm_functions.txt` lists them
(hand-written, with portable C beside them). "Code" also holds code in dsd gap units that
has no function, so it is a little larger than the totals above (issue 0071).

| Module | Code | Decompiled | Fully linked | Functions matched |
|---|---:|---:|---:|---:|
| main | 1216386 | 100.00% | 100.00% | 6004 / 6004 |
| itcm | 24184 | 100.00% | 100.00% | 117 / 117 |
| ov000 | 25300 | 100.00% | 100.00% | 120 / 120 |
| ov001 | 7472 | 100.00% | 100.00% | 76 / 76 |
| ov002 | 64208 | 100.00% | 100.00% | 164 / 164 |
| ov003 | 82696 | 100.00% | 100.00% | 235 / 235 |
| ov004 | 67364 | 100.00% | 100.00% | 236 / 236 |
| ov005 | 96684 | 100.00% | 100.00% | 266 / 266 |
| ov006 | 105588 | 100.00% | 100.00% | 321 / 321 |
| arm7 | 155140 | 99.96% | 100.00% | 1028 / 1032 |
| all | 1845022 | 100.00% | 100.00% | 8567 / 8571 |

## Partial functions

Functions in C files that the build does not use yet (not marked `complete`), because
they do not match. Each one is compiled and compared with the original by `fn.py diff`:
"Differ" counts the rows of its aligned listing that differ (an instruction only one side
has counts as one row; a relocation naming another symbol at the same address does not
count). "Match" is objdiff's percentage. The list ranks by "Differ", then "Match":
objdiff's score charges one wrong opcode as much as 12 wrong registers (issue 0067).

None.
