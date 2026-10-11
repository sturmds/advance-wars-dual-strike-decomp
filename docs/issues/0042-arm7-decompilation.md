# 0042: ARM7 has no C at all

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** later

## Problem

All decompiled code so far is ARM9 (main, itcm and the overlays). The ARM7 binary (157,268 bytes at 0x02380000) is copied from `extract/` unchanged, and none of it is in C. The hard rules in `CLAUDE.md` keep it that way "until a later milestone". No issue tracks what that milestone needs.

## Notes

- Issue 0013 covers the build side: linking `arm7.bin` from delinked objects instead of copying it. That has to come first. This issue covers the step after it: replacing those objects with C.
- 0013 notes the string "2.39.00" and that ARM7 looks like the standard ARM7 component shipped to every DS game (the second CPU, which runs sound, touch screen, wireless and power). If it is unmodified library code, decompiling it is library work, closer to the library code in main and itcm than to game code.

Scope to decide:

1. Is any of ARM7 game code, or is it all stock SDK? Compare against other DS games that ship SDK 2.39 ARM7 components, and look for references to game data.
2. Which compiler and flags built it. It may not be mwccarm 2.0-72 with the ARM9 flags; ARM7 code targets the ARM7TDMI and is often Thumb.
3. Tooling. Whether dsd can delink and check ARM7 the way it does ARM9, or what we would add (0013 says `init` and `lcf` appear to handle ARM9 only).
4. Priority against the remaining ARM9 work: the partials and the ARM9 SDK/library code (742 functions).

## Done when

The questions above are answered here, and a plan exists for ARM7 in C after 0013 is done. Close or split this issue then.
