#ifndef GAME_MAP_BG_H
#define GAME_MAP_BG_H

#include "types.h"

/* BG tile map entries: bits 0-9 character, 10-11 flips, 12-15 palette. */
#define BG_TILE_CHAR_AND_FLIP 0xfff
#define BG_TILE_PALETTE(n) ((n) << 12)

/* Tables used to draw map tiles into the BG tile maps of layers 3 and 2
 * (MapBg_DrawTileRow, MapBg_DrawTileColumn). Each map tile is 2x2 BG
 * tiles. */
extern u16
    gMapCellGroundTiles[]; /* 4 BG tile entries per map cell, for layer 3 */
extern u16 gMapCellOverhangTiles
    []; /* same for layer 2: the part of a cell drawn into the row above */
extern u16
    gMountainCellVariants[16]; /* mountain cells, picked by tile position */
extern u16 data_02169e78[32];  /* per terrain id; 0: keep the map's cell */
extern u16 data_02169eb8
    [32]; /* tile map offset of each map tile column: two 32x32 screen blocks */

#endif
