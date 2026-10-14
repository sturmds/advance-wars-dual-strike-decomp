#ifndef GAME_MAP_BG_H
#define GAME_MAP_BG_H

#include "types.h"

/* Map tiles (16x16 pixels) the screen shows, 256 / 16 by 192 / 16. A
 * redraw covers one more column and row, for a view between tiles. */
#define MAP_VIEW_COLUMNS 16
#define MAP_VIEW_ROWS 12

/* gOwnerCellCodes, rows of owner variants of a map cell, ends with this. */
#define OWNER_CELL_END 0xffff

/* BG tile map entries: bits 0-9 character, 10-11 flips, 12-15 palette. */
#define BG_TILE_CHAR_AND_FLIP 0xfff
#define BG_TILE_HFLIP 0x400
#define BG_TILE_PALETTE(n) ((n) << 12)

/* Tables used to draw map tiles into the BG tile maps of layers 3 and 2
 * (MapBg_DrawTileRow, MapBg_DrawTileColumn). Each map tile is 2x2 BG
 * tiles. */
extern const u16
    gMapCellGroundTiles[]; /* 4 BG tile entries per map cell, for layer 3 */
extern const u16 gMapCellOverhangTiles
    []; /* same for layer 2: the part of a cell drawn into the row above */
extern u16
    gMountainCellVariants[]; /* mountain cells, picked by tile position */
extern u16 data_02169e78[];  /* per terrain id; 0: keep the map's cell */
extern u16 data_02169eb8
    []; /* tile map offset of each map tile column: two 32x32 screen blocks */

#endif
