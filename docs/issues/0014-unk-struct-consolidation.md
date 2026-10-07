# 0014: `Unk_*` structs need merging and real names

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-07
- **Milestone:** M2

## Problem

Each matched function that reads through a pointer got its own `include/game/Unk_<address>.h`. Several of them probably describe the same struct seen from different functions (the chain behind `data_027c027c` is used by func_02080440 and func_020802e8 and hundreds of others).

## Notes

- A readability pass: find headers whose structs are reached from the same global, merge their fields, give the struct a real name once its role is clear, and `fn.py rename` the globals.
- Do it after more functions match, so more fields are known.

## Done when

No two headers describe the same struct, and the most used structs have real names.
