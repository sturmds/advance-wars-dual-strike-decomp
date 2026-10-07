# 0006: The built ROM has not been run in an emulator

- **Status:** blocked
- **Area:** emulation
- **Opened:** 2026-10-07
- **Milestone:** M3

## Problem

The M0 definition of done includes "boots in an emulator". `build/AWRE.nds` is byte-identical to retail, so it boots exactly as retail does, but nobody has run it. Once C changes code layout (modding), only an emulator can test the game.

## Notes

- Blocked: no emulator is installed, and sturmds is not running apt for now.
- M3 plan: a Linux emulator with Lua scripting and a GDB stub (melonDS, DeSmuME or BizHawk), scripted inputs, and RAM or screenshot comparisons against retail.
- Options that avoid apt (a pinned AppImage in `tools.lock`, say) are possible but count as installing a tool: ask first.

## Done when

The built ROM boots to the title screen in a pinned emulator, run from a `make` target.
