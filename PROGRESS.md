# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 167 / 6603 | 23812 / 1531944 | 1.55% | 0 | 0 |
| SDK/library | 1 / 880 | 40 / 154050 | 0.03% | 1 | 30 |
| all | 168 / 7483 | 23852 / 1685994 | 1.41% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 164 / 5180 | 23644 / 1082632 | 2.18% | 0 |
| main | SDK/library | 1 / 767 | 40 / 130618 | 0.03% | 1 |
| ov003 | game | 2 / 235 | 64 / 82696 | 0.08% | 0 |
| ov006 | game | 1 / 321 | 104 / 105588 | 0.10% | 0 |

Modules with nothing done: itcm, ov000, ov001, ov002, ov004, ov005.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

None.
