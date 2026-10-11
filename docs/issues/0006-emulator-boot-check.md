# 0006: The built ROM has not been run in an emulator

- **Status:** open
- **Area:** emulation
- **Opened:** 2026-10-07
- **Milestone:** M3

## Problem

The M0 definition of done includes "boots in an emulator". `build/AWRE.nds` is byte-identical to retail, so it boots exactly as retail does, but nobody has run it. Once C changes code layout (modding), only an emulator can test the game.

## Notes

- ~~Blocked: no emulator is installed~~. Since 2026-10-11 DeSmuME 0.9.13 is installed as `/usr/games/desmume-cli` (system package, not in `tools.lock`). Its `--help` lists no GDB stub and no Lua; it does have `--play-movie DSM_FILE` (replay recorded input), `--load-slot N` (savestates), `--start-paused` and `--disable-limiter`. `/usr/games/mgba` is GBA only.
- `make emu-smoke` (`tools/emu_smoke.py`, 2026-10-11) runs retail, `build/AWRE.nds` and the shifted build (`make shift`, issue 0012) for 20 s each with `SDL_VIDEODRIVER=offscreen` and `SDL_AUDIODRIVER=dummy`, so no window opens (the `dummy` video driver fails: DeSmuME wants OpenGL). HOME points at `build/emu/`, so saves stay out of the real home. With no input, the game formats its 256 KiB save in the first seconds; the test compares that save's SHA-1, the exit status (124: still running at the timeout) and the log. Result: all three ROMs, and a 0x1000-byte shift, give the same save and status.
- **This is not enough testing.** A ROM with main's code at 0x02010000-0x02013fff overwritten with 0xFF passed it too; only one with 960 KiB overwritten failed (no save written). A hung game, a wrong pointer in a menu or a crash after the save is formatted all pass. Nothing reads pixels (no screenshots, by sturmds's rule) or RAM.
- Options for the real test, for sturmds to choose (each counts as installing a tool):
  - DeSmuME as installed, plus input movies: record a `.dsm` once, replay it with `--play-movie`. Gives scripted input, but desmume-cli has no way to dump RAM or state at a checkpoint, so it still sees only the save file. Nothing to install.
  - melonDS: has a GDB stub for ARM9 and ARM7 (enable in its settings), so a script can set breakpoints, read RAM and compare with retail. No Lua. Pin the Linux x86_64 AppImage from the melonDS GitHub releases in `tools.lock` (URL plus SHA-256, fetched by `tools/setup.py` like dsd), or `sudo apt install melonds` if the Ubuntu archive has it for this release (check with `apt policy melonds`).
  - BizHawk: Lua scripting with its melonDS core, Linux build on .NET/Mono. Most features, heaviest install; pin its release archive in `tools.lock`.
- M3 plan: a Linux emulator with Lua scripting and a GDB stub (melonDS, DeSmuME or BizHawk), scripted inputs, and RAM or screenshot comparisons against retail.

## Done when

The built ROM boots to the title screen in a pinned emulator, run from a `make` target.

Not done yet: DeSmuME is not pinned, and the smoke test cannot see the title screen.
