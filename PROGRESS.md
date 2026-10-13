# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
The numbers come from `build/artifacts/AWRE_report/report.json`,
the report `make release` writes for decomp.dev (`tools/decomp_dev_report.py`).
"Decompiled" is the code that matches the ROM from C; "Fully linked" is the part of it
that the build links from `src/` (files marked `complete` in `delinks.txt`). Functions
written as `asm` count only when `config/AWRE/asm_functions.txt` lists them. Bytes count
code only, including code in dsd gap units that has no function (issue 0071).

| Code | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |
|---|---:|---:|---:|---:|---:|
| all | 100.0000% | 100.0000% | 1845012 / 1845012 | 1845012 / 1845012 | 8571 / 8571 |

## By module

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`.

| Module | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |
|---|---:|---:|---:|---:|---:|
| main | 100.0000% | 100.0000% | 1216386 / 1216386 | 1216386 / 1216386 | 6004 / 6004 |
| itcm | 100.0000% | 100.0000% | 24184 / 24184 | 24184 / 24184 | 117 / 117 |
| ov000 | 100.0000% | 100.0000% | 25300 / 25300 | 25300 / 25300 | 120 / 120 |
| ov001 | 100.0000% | 100.0000% | 7472 / 7472 | 7472 / 7472 | 76 / 76 |
| ov002 | 100.0000% | 100.0000% | 64208 / 64208 | 64208 / 64208 | 164 / 164 |
| ov003 | 100.0000% | 100.0000% | 82696 / 82696 | 82696 / 82696 | 235 / 235 |
| ov004 | 100.0000% | 100.0000% | 67364 / 67364 | 67364 / 67364 | 236 / 236 |
| ov005 | 100.0000% | 100.0000% | 96684 / 96684 | 96684 / 96684 | 266 / 266 |
| ov006 | 100.0000% | 100.0000% | 105588 / 105588 | 105588 / 105588 | 321 / 321 |
| arm7 | 100.0000% | 100.0000% | 155130 / 155130 | 155130 / 155130 | 1032 / 1032 |

## By kind of code

Gap units are code and data that dsd delinks with no source file.

| Kind | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |
|---|---:|---:|---:|---:|---:|
| game | 100.0000% | 100.0000% | 1556364 / 1556364 | 1556364 / 1556364 | 6749 / 6749 |
| SDK/library | 100.0000% | 100.0000% | 288648 / 288648 | 288648 / 288648 | 1822 / 1822 |

## Below 100%

Symbols in code that the report scores below 100%, by module.

None.
