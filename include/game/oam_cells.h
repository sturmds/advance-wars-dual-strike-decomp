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

/* Cell lists of two, three and four cells. The lists in DTCM start on
 * 4-byte boundaries, and the ROM pads the two- and four-cell lists with
 * two zero bytes. */
typedef struct OamCellList2 {
  s16 count; /* 2 */
  OamCell cells[2];
  u16 pad;
} OamCellList2;

typedef char OamCellList2_size_check[sizeof(OamCellList2) == 16 ? 1 : -1];

typedef struct OamCellList3 {
  s16 count; /* 3 */
  OamCell cells[3];
} OamCellList3;

typedef char OamCellList3_size_check[sizeof(OamCellList3) == 20 ? 1 : -1];

typedef struct OamCellList4 {
  s16 count; /* 4 */
  OamCell cells[4];
  u16 pad;
} OamCellList4;

typedef char OamCellList4_size_check[sizeof(OamCellList4) == 28 ? 1 : -1];

/* The cell lists in DTCM (dtcm_data_027c0000.c). One cell, by sprite
 * size in pixels: */
extern OamCellList1 data_027c0000; /* 16x32 */
extern OamCellList1 data_027c0008; /* 64x32 */
extern OamCellList1 data_027c0010; /* 8x32 */
extern OamCellList1 data_027c0018; /* 32x64 */
extern OamCellList1 data_027c0020; /* 32x8 */
extern OamCellList1 data_027c0028; /* 8x8 */
extern OamCellList1 data_027c0030; /* 64x64 */
extern OamCellList1 data_027c0038; /* 8x16 */
extern OamCellList1 data_027c0040; /* 16x16 */
extern OamCellList1 data_027c0048; /* 32x32 */
extern OamCellList1 data_027c0050; /* 8x8 */
extern OamCellList1 data_027c0058; /* 16x8 */
extern OamCellList1 data_027c0060; /* 32x16 */
/* Two cells */
extern OamCellList2 data_027c0068;
extern OamCellList2 data_027c0078;
extern OamCellList2 data_027c0088;
extern OamCellList2 data_027c0098;
extern OamCellList2 data_027c00a8;
extern OamCellList2 data_027c00b8;
extern OamCellList2 data_027c00c8;
extern OamCellList2 data_027c00d8;
extern OamCellList2 data_027c00e8;
extern OamCellList2 data_027c00f8;
extern OamCellList2 data_027c0108;
extern OamCellList2 data_027c0118;
extern OamCellList2 data_027c0128;
extern OamCellList2 data_027c0138;
/* Three cells */
extern OamCellList3 data_027c0148;
extern OamCellList3 data_027c015c;
extern OamCellList3 data_027c0170;
extern OamCellList3 data_027c0184;
extern OamCellList3 data_027c0198;
extern OamCellList3 data_027c01ac;
/* Four cells */
extern OamCellList4 data_027c01c0;
extern OamCellList4 data_027c01dc;

#endif
