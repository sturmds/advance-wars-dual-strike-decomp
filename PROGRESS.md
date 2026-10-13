# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
The numbers come from `build/artifacts/0.1_report/report.json`,
the report `make release` writes for decomp.dev (`tools/decomp_dev_report.py`).
"Decompiled" is the code that matches the ROM from C; "Fully linked" is the part of it
that the build links from `src/` (files marked `complete` in `delinks.txt`). Functions
written as `asm` count only when `config/AWRE/asm_functions.txt` lists them. Bytes count
code only, including code in dsd gap units that has no function (issue 0071).

| Code | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |
|---|---:|---:|---:|---:|---:|
| all | 100.00% | 100.00% | 1844958 / 1845022 | 1845022 / 1845022 | 8567 / 8571 |

## By module

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`.

| Module | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |
|---|---:|---:|---:|---:|---:|
| main | 100.00% | 100.00% | 1216386 / 1216386 | 1216386 / 1216386 | 6004 / 6004 |
| itcm | 100.00% | 100.00% | 24184 / 24184 | 24184 / 24184 | 117 / 117 |
| ov000 | 100.00% | 100.00% | 25300 / 25300 | 25300 / 25300 | 120 / 120 |
| ov001 | 100.00% | 100.00% | 7472 / 7472 | 7472 / 7472 | 76 / 76 |
| ov002 | 100.00% | 100.00% | 64208 / 64208 | 64208 / 64208 | 164 / 164 |
| ov003 | 100.00% | 100.00% | 82696 / 82696 | 82696 / 82696 | 235 / 235 |
| ov004 | 100.00% | 100.00% | 67364 / 67364 | 67364 / 67364 | 236 / 236 |
| ov005 | 100.00% | 100.00% | 96684 / 96684 | 96684 / 96684 | 266 / 266 |
| ov006 | 100.00% | 100.00% | 105588 / 105588 | 105588 / 105588 | 321 / 321 |
| arm7 | 99.96% | 100.00% | 155076 / 155140 | 155140 / 155140 | 1028 / 1032 |

## By kind of code

Gap units are code and data that dsd delinks with no source file.

| Kind | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |
|---|---:|---:|---:|---:|---:|
| game | 100.00% | 100.00% | 1556364 / 1556364 | 1556364 / 1556364 | 6749 / 6749 |
| SDK/library | 99.98% | 100.00% | 288594 / 288658 | 288658 / 288658 | 1818 / 1822 |

## Below 100%

Symbols in code that the report scores below 100%, by module.

| Module | Unit | Symbol | Bytes | Match |
|---|---|---|---:|---:|
| arm7 | `src/arm7/sdk/card/card_request_tables` | gCardSaveCmdReadStatus | 4 | 40.00% |
| arm7 | `src/arm7/sdk/card/card_request_tables` | gCardSaveCmdWriteEnable | 4 | 40.00% |
| arm7 | `src/arm7/sdk/snd/snd_ex_channel_tables` | gSndAttackTable | 20 | 88.00% |
| arm7 | `src/arm7/sdk/snd/snd_volume_tables` | gSndSineQuarterTable | 36 | 93.33% |
