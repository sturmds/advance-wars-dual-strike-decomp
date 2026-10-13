# zeldaret/oot

- **Project:** oot, a matching decompilation of *The Legend of Zelda: Ocarina of Time* (N64) by the Zelda Reverse Engineering Team (zeldaret).
- **Authors:** the zeldaret contributors.
- **URL:** https://github.com/zeldaret/oot
- **Version we read:** commit `ceeb6f35cf690faf6bd96b33e14e4b45ce35441e` (2026-10-08).
- **License:** none stated in the repository (no LICENSE file, nothing in the README), so we take the idea only.

## What we take

The pattern, no code or data: game data that must not be committed is kept in C files as array definitions whose initializers `#include` a generated `.inc.c` file, and the build writes those files from the builder's own copy of the game (their `assets/objects/object_link_child/object_link_child.c` includes `gLinkChildEyesOpenTex.ci8.tlut_gLinkChildSkinTLUT.inc.c` and others; their `Makefile` makes the `.inc.c` files from extracted assets). Used by `tools/data_inc.py`, `config/AWRE/data_inc.txt` and the C files that include `data/...inc.c` (`docs/workflow.md`, "Data from the ROM").

## What we do not take

Their tools, Makefile rules, C, headers and names. It is a different game on a different console and compiler.
