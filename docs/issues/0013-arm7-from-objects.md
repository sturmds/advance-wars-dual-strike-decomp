# 0013: ARM7 is copied from the ROM, not built from objects

- **Status:** closed
- **Area:** build
- **Opened:** 2026-10-07
- **Milestone:** M2

## Problem

D1 says the build must eventually produce ARM7 from objects (delinked bytes plus any C). Today `arm7.bin` comes from `extract/` unchanged.

## Notes

- dsd's `init` and `lcf` handle ARM9 only; see the resolution.
- ARM7 (157,268 B at 0x02380000) contains "2.39.00" and looks like the standard ARM7 component shipped to every DS game, unchanged by this game.
- Decompiling ARM7 to C, after this issue, is tracked in 0042.

## Resolution

Closed 2026-10-11. dsd 0.12.1 cannot delink ARM7 (`docs/tool-issues.md`, "dsd: no ARM7 programs"), so `tools/arm7.py` runs it on a stand-in ROM with the ARM7 parts moved into its address range, and links the objects at their real addresses. `config/AWRE/arm7/` came from `tools/arm7_init.py`. `build/build/arm7.bin` is linked from 1,014 delinked functions plus data, its SHA-1 is in `config/AWRE/modules.sha1`, and `make check` checks it before the whole ROM.

## Done when

`arm7.bin` is linked from objects and `make release` still matches.
