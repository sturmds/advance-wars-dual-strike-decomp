# 0023: A separate mod build on top of the matching source

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-09
- **Milestone:** later

## Problem

sturmds wants mods (first: War Room army colour from the CO, and both maps shown in two-map battles). This repo builds a byte-for-byte copy of retail AWRE, and that stays its only output: mods must never change what `make release` checks, and no mod `#ifdef`s go into this repo.

## Notes

- Proposal: a second repo in `~/repos/` (for example `advance-wars-dual-strike-decomp-mods`) that holds this repo as a git submodule pinned to a mainline commit, applies its changes and builds an unchecked ROM into its own build directory.
- How changes apply, to decide: patch files on a copy of the submodule tree (simple, break when mainline moves), or replacement files at the same paths that the mod build prefers (no patch fuzz, whole-file copies drift). `#ifdef MOD_...` here is rejected.
- Needs a shiftable build: a mod that changes code size moves every later address, so every pointer must be a symbol (plan, Milestone 3). Until then only same-size or data-only mods work.
- Check that the delink step still treats a modded file as C and that `dsd rom build` accepts modules whose size changed. 2026-10-11: `make shift` (issue 0012) shows `dsd rom build` accepts a larger main and moves the overlays after it; the secure-area CRC is then wrong (issue 0056). `tools/shift_build.py` shows how to link a changed copy into its own build directory.
- Same rules as here: local only, no ROM or built output in git, everything through make, tools from `tools.lock`.
- First mod: `Map_LoadHeaderSetup` in War Room mode sets `army[p]` from `CoRecord.armyStyle + 1`; two-map battles get it per state; the map select preview needs it too. It grows code in `main`, so it needs the shiftable build. See issue 0022 for the style/colour facts.

## Done when

The mod repo builds a playable ROM with the War Room colour change from a pinned mainline commit, without any change to this repo's output.
