# pokeheartgold

- **Project:** pokeheartgold, the pret decompilation of *Pokémon HeartGold* and *SoulSilver* (DS). It includes a decompiled copy of the Nitro SDK that the game links.
- **Authors:** the pret contributors named in the repo's history.
- **URL:** https://github.com/pret/pokeheartgold (the `origin` remote of our local copy).
- **Version we read:** commit `9d8b7591` (2026-09-20), local copy in `~/repos/pokeheartgold`.
- **License:** none stated in the repo.

## What we take

Names only: `SDK_MAIN_ARENA_LO` and `SDK_SECTION_ARENA_ITCM_START`, the linker symbols that its copy of the SDK arena code (`os_arena.c`) gives `OS_GetInitArenaLo` for the main and ITCM arenas. We use them for the same symbols in our linker script (issue 0086). No code, comments or files come from pokeheartgold.
