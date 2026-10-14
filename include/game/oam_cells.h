#ifndef GAME_OAM_CELLS_H
#define GAME_OAM_CELLS_H

#include "types.h"

#define OAM_ATTR1_HFLIP 0x1000
#define OAM_ATTR1_FLIPS 0x3000 /* horizontal and vertical flip */

/* One OBJ of a sprite: OAM attributes 0-2, with the position relative to
 * the sprite and the tile number relative to its first tile. A cell list
 * holds an s16 count, then that many cells; the tables Oam_GetShapeCells
 * returns hold one cell each. */
typedef struct OamCell {
  u16 attr0; /* bits 0-7: y */
  u16 attr1; /* bits 0-8: x */
  u16 attr2; /* bits 0-9: tile */
} OamCell;

typedef char OamCell_size_check[sizeof(OamCell) == 6 ? 1 : -1];

/* A cell list of one cell: one OBJ of a given OAM shape and size at the
 * sprite's origin (Oam_GetShapeCells, Oam_GetAttrCells). */
typedef struct OamCellList1 {
  s16 count; /* 1 */
  OamCell cell;
} OamCellList1;

typedef char OamCellList1_size_check[sizeof(OamCellList1) == 8 ? 1 : -1];

/* One-cell lists in DTCM (dtcm_data_027c0000.c) */
extern OamCellList1 data_027c0000; /* 16x32 */
extern OamCellList1 data_027c0008; /* 64x32 */
extern OamCellList1 data_027c0010; /* 8x32 */
extern OamCellList1 data_027c0018; /* 32x64 */
extern OamCellList1 data_027c0020; /* 32x8 */

#endif
