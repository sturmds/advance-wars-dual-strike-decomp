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
- 2026-10-14: `make emu-check` (`tools/melonds_check.py`, `docs/workflow.md`) boots each ROM in melonDS 1.1 (a local build with its GDB stub, a system dependency that sturmds chose not to pin) to the main menu, with no window and no sound, and stops on an ARM9 exception, a hang or a heap that starts inside overlay memory. Retail, `build/AWRE.nds` and the shifts of main, ov000, itcm and main+ov000 pass; each takes about 14 s, four run at a time. Issue 0086's crash ROM (main and ov000 grown by 0x40, the old `OS_GetInitArenaLo`) fails: heap start 0x0237a5a0 before the overlays' end 0x0237a620, then a data abort at 0x022bb22c (`Ov004_SetSlotTask`) at frame 2930, once START is pressed on the title screen; with no input it plays the opening without a crash. It also found that the dtcm shift dies at its first instructions: `init_cp15` builds the DTCM protection region from the address of the first DTCM object, so a 0x20 pad gives a 1 GiB region over main RAM and a prefetch abort (issue 0086).
- 2026-10-14: `make emu-smoke` and `tools/emu_smoke.py` are gone (sturmds): `make emu-check` covers what they checked. `make test` no longer starts melonDS either; only `make emu-check` does, since the decomp loop does not need an emulator.
- M3 plan: a Linux emulator with Lua scripting and a GDB stub (melonDS, DeSmuME or BizHawk), scripted inputs, and RAM or screenshot comparisons against retail.

## Done when

The built ROM boots to the title screen in a pinned emulator, run from a `make` target.

Not done yet: `make emu-check` reaches the main menu, but melonDS is a local build, not pinned in `tools.lock`. sturmds decides whether that is enough to close this issue.
