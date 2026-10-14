# zxtune

- **Project:** zxtune, a player and converter for chiptune and game music formats. Its command-line player, zxtune123, plays Nintendo DS music (NCSF, 2SF, STRM) and writes WAV.
- **Authors:** Vitamin/CAIG and contributors. The DS sequence player inside it is SSEQPlayer, by kode54 and CyberBotX, from the FeOS Sound System and DeSmuME (`3rdparty/sseqplayer/`).
- **URL:** https://github.com/vitamin-caig/zxtune
- **Version we use:** r5112 (2026-10-02), the prebuilt `zxtune_r5112_linux_x86_64.tar.gz` from the GitHub release, pinned in `tools.lock` with its SHA-256. The hash matches the digest GitHub lists for the asset. `make setup` unpacks it into `.cache/tools/zxtune/`.
- **License:** LGPL-3.0 (`LICENSE.md`; GitHub reports the same). SSEQPlayer is under the WTFPL (`3rdparty/sseqplayer/LICENSE.TXT`).
- **DS support:** the project's features page does not list the DS formats; its changelog does: 2SF in r3950 (2017), NCSF in r4990 (2021), and fixes for both in r5110 (2026). `zxtune123 --list-plugins` lists `NCSF` and `STRM`.

## How we use it

`tools/sound.py export` (`make music`, `docs/sound.md`) runs zxtune123 unchanged, as a program: `zxtune123 --wav filename=out --silent FILE`. zxtune123 needs only zlib and the C library. We do not use the Qt player (`zxtune-qt`) or `xtractor`, which come in the same archive.

The NCSF layout that `export` writes (PSF version 0x25, a 4-byte sequence number in the reserved area, an SDAT as the program, `_lib`, `length` and `fade` tags) was taken from zxtune's reader, `src/formats/chiptune/emulation/nitrocomposersoundformat.cpp`, and from CyberBotX's in_xsf (https://github.com/CyberBotX/in_xsf). We checked it by rendering our files.

## What we do not take

None of its code is copied into this repo. The audio it renders is game data and stays in `extracted/` or `build/`.
