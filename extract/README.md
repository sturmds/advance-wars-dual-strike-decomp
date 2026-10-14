# extract/

The ROM split into its parts by `dsd rom extract`, as the build's input. `make extract` writes it from `orig/AWRE.nds` (and `make` does when it is missing or the ROM changed). Nothing here is edited by hand, and nothing here except this file and `.gitkeep` enters git: it is game data (tenet 9).

| Path | What |
|---|---|
| `arm9/`, `arm7/`, `arm9_overlays/` | The code modules as binaries: main, ITCM, DTCM, the overlays, ARM7. `dsd delink` cuts the original objects from these. |
| `files/` | The ROM's file system (graphics, text, `data/sound_data.sdat`, ...). The build copies it into the new ROM unchanged (issue 0050). |
| `config.yaml`, `header.yaml`, `banner/`, `header_logo.png`, `path_order.txt` | dsd's description of the ROM, which `dsd rom build` uses to put a ROM back together. |

`make extract` replaces everything in this directory except `README.md` and `.gitkeep`.

Not to be confused with `extracted/`, which holds forms of the game data that our own tools write for people to read and edit. The two may be renamed later.
