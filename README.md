# Advance Wars: Dual Strike decompilation

A matching decompilation of *Advance Wars: Dual Strike* for the Nintendo DS, US/Australia cartridge (game code `AWRE`). The C in this repo, compiled with the original Metrowerks compiler and flags, rebuilds the retail ROM byte for byte. `PROGRESS.md` has how much of the code is decompiled and linked, as decomp.dev measures it.

This repo holds no game data. You need your own dump of the cartridge; the build unpacks what it needs from it.

## Requirements

Linux, without Wine: the Metrowerks compiler and linker run through metroskrew, whose binaries are 32-bit (hence `libc6:i386`). Packages, on Ubuntu:

```sh
sudo apt install make ninja-build python3 python3-yaml libc6:i386
```

`make setup` downloads the pinned tools in `tools.lock` (dsd, objdiff, metroskrew, clang-format) into `.cache/` and checks their SHA-256.

## Building

Your ROM must be the US/Australia retail cartridge, SHA-1 `5a8d22d41caa2c0ff41fb4a2b885a6be607e22d9` (`config/AWRE/rom.sha1`).

```sh
mkdir -p orig && ln -s /path/to/your/awds.nds orig/AWRE.nds
make setup    # tools, git hooks, and the game data the build needs, unpacked from your ROM into extracted/
make          # build/AWRE.nds
make check    # compare it with the retail ROM, module by module and as a whole
```

`make release` runs all three plus the format check, and every commit has to pass it.

## Layout

| Path | What |
|---|---|
| `src/`, `include/` | The C and headers: `src/game/` for the game, `src/sdk/` for library code, `src/arm7/` for the second CPU. |
| `config/AWRE/` | dsd configuration: how the ROM splits into modules, sections and symbols. |
| `assets/` | How to read game data the build unpacks from your ROM (for the maps: the tile legend and the order files). Never the data itself. |
| `tools/` | Build, matching and checking tools; `tools/configure.py` writes `build.ninja`. |
| `docs/` | The plan, the workflow, notes on the game's data, the issue tracker and credits (`docs/third-party/`). |
| `TENETS.md` | The rules every change is checked against. |

## How it was made

Most of the C was written by Claude (Anthropic's model) in an automated matching loop and checked by the build, which must reproduce the retail bytes, and by a review against `TENETS.md`. Commits made that way carry a `Co-Authored-By: Claude` trailer. `docs/loop.md` and `docs/workflow.md` describe the process.

## Credits

Outside sources and what we took from each are listed in `docs/third-party/`: names and data layouts from RadioTails' romhacking.net document 890, tool ideas from khdays-decomp, the pattern for data extracted at setup and included by C from zeldaret/oot, hardware names from GBATEK and libnds, and the tools pinned in `tools.lock`.

## License

CC0 1.0 Universal (`LICENSE`). This covers the code and documents written for this repo, not the game, which belongs to its rights holders. This project is not affiliated with or endorsed by Nintendo.
