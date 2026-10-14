# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
The numbers come from `build/artifacts/AWRE_report/report.json`,
the report `make release` writes for decomp.dev (`tools/decomp_dev_report.py`).
"Code decompiled" is the code that matches the ROM from C; "Code linked" is the part of it
that the build links from `src/` (files marked `complete` in `delinks.txt`). Functions
written as `asm` count only when `config/AWRE/asm_functions.txt` lists them. Code bytes
include code in dsd gap units that has no function (issue 0071). "Data linked" is the
data (initialised data, read-only data and .bss) in files the build links from `src/`;
the rest is in dsd gap units, delinked from the ROM with no source file.

| Code | Code decompiled | Code linked | Bytes decompiled | Bytes linked | Functions matched | Data linked | Data bytes linked |
|---|---:|---:|---:|---:|---:|---:|---:|
| all | 100.0000% | 100.0000% | 1845012 / 1845012 | 1845012 / 1845012 | 8571 / 8571 | 94.9086% | 2976452 / 3136124 |

## By module

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`.

| Module | Code decompiled | Code linked | Bytes decompiled | Bytes linked | Functions matched | Data linked | Data bytes linked |
|---|---:|---:|---:|---:|---:|---:|---:|
| main | 100.0000% | 100.0000% | 1216386 / 1216386 | 1216386 / 1216386 | 6004 / 6004 | 96.8099% | 1538032 / 1588712 |
| itcm | 100.0000% | 100.0000% | 24184 / 24184 | 24184 / 24184 | 117 / 117 | - | 0 / 0 |
| dtcm | - | - | 0 / 0 | 0 / 0 | 0 / 0 | 100.0000% | 4832 / 4832 |
| ov000 | 100.0000% | 100.0000% | 25300 / 25300 | 25300 / 25300 | 120 / 120 | 83.0321% | 533352 / 642344 |
| ov001 | 100.0000% | 100.0000% | 7472 / 7472 | 7472 / 7472 | 76 / 76 | 100.0000% | 164608 / 164608 |
| ov002 | 100.0000% | 100.0000% | 64208 / 64208 | 64208 / 64208 | 164 / 164 | 100.0000% | 48224 / 48224 |
| ov003 | 100.0000% | 100.0000% | 82696 / 82696 | 82696 / 82696 | 235 / 235 | 100.0000% | 21408 / 21408 |
| ov004 | 100.0000% | 100.0000% | 67364 / 67364 | 67364 / 67364 | 236 / 236 | 100.0000% | 613568 / 613568 |
| ov005 | 100.0000% | 100.0000% | 96684 / 96684 | 96684 / 96684 | 266 / 266 | 100.0000% | 15520 / 15520 |
| ov006 | 100.0000% | 100.0000% | 105588 / 105588 | 105588 / 105588 | 321 / 321 | 100.0000% | 14272 / 14272 |
| arm7 | 100.0000% | 100.0000% | 155130 / 155130 | 155130 / 155130 | 1032 / 1032 | 100.0000% | 22636 / 22636 |

## By kind of code

Gap units are code and data that dsd delinks with no source file.

| Kind | Code decompiled | Code linked | Bytes decompiled | Bytes linked | Functions matched | Data linked | Data bytes linked |
|---|---:|---:|---:|---:|---:|---:|---:|
| game | 100.0000% | 100.0000% | 1556364 / 1556364 | 1556364 / 1556364 | 6749 / 6749 | 100.0000% | 2936260 / 2936260 |
| SDK/library | 100.0000% | 100.0000% | 288648 / 288648 | 288648 / 288648 | 1822 / 1822 | 100.0000% | 40192 / 40192 |
| gap units | - | - | 0 / 0 | 0 / 0 | 0 / 0 | 0.0000% | 0 / 159672 |

## Below 100%

Symbols in code that the report scores below 100%, by module.

None.

Units with data (from `src/`) whose data the report scores below 100% matched.

None.
