# 0013: ARM7 is copied from the ROM, not built from objects

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-07
- **Milestone:** M2

## Problem

D1 says the build must eventually produce ARM7 from objects (delinked bytes plus any C). Today `arm7.bin` comes from `extract/` unchanged.

## Notes

- dsd's `init` and `lcf` appear to handle ARM9 only, so this path is likely our own work.
- ARM7 (157,268 B at 0x02380000) contains "2.39.00" and looks like the standard ARM7 component shipped to every DS game, unchanged by this game.

## Done when

`arm7.bin` is linked from objects and `make release` still matches.
