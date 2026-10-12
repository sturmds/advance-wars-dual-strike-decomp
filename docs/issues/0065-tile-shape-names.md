# 0065: Tile tokens for shapes are numbers, not names

- **Status:** open
- **Area:** docs
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

`assets/maps/legend.yaml` names each tile code from the terrain id and owner that `/bmap/003` gives it (`docs/maps.md`). Terrain with one code per owner reads well (`C1`, `H2`, `B0`). Terrain drawn with many shapes does not: the most used code gets the bare symbol and the rest are numbered in code order, so `o23` or `=7` says nothing about which coast, corner or bend it is. In `bmap003` that is 53 sea codes, 40 river, 36 shoal, 22 road, 10 pipe, 5 plain and 17 large-object parts (`x`); in `bmap002`, the Grand Bolt's body `G1`-`G177`.

## Notes

- The shape of a code is in the tile graphics, not in the code table. The map renderer turns a code into tiles (`MapTile3d_GetTextureOffset`, the 2D path through `data_021587e0` and `data_02149080` in `MapPreview_DrawBlock`); following that to the graphics files would show each code's picture.
- Neighbour patterns in the maps can help: a sea code that always has land to its north is a north coast.
- A naming scheme to aim for: the symbol plus the sides that meet another terrain (`o_n`, `o_ne`, `=_ns`, `=_ew`, `~_nes`). Tokens may be longer than 3 characters; `unpack` widens the columns to fit.
- Renaming only touches `legend.yaml`; `python3 tools/maps.py unpack` then rewrites the maps with the new tokens, and the ROM bytes do not change.

## Done when

Every sea, river, shoal, road and pipe code in `bmap003` has a token that says its shape, `docs/maps.md` gives the scheme, and `make release` passes.
