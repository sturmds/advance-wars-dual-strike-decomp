#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gSavedArmyColours[];
extern u16 gMapPreviewColumnOffsets[];
extern u16 gMapPreviewRowOffsets[];
extern u16 data_021587e0[];
extern u16 data_02139d90[];
/* One u16 (four 4bpp pixels) per row of a 4x4 cell: the pixels a unit dot keeps. */
extern u16 gMapPreviewBlockUnitMask[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Unit layer of one block of the map preview, drawn beside the terrain
 * block of MapPreview_DrawBlock: clears the cell of each of width by height
 * tiles from (x, y) on, then draws a dot in the owner's army colour where a
 * unit stands, as MapPreview_DrawUnits does. */
void MapPreview_DrawBlockUnits(u16* dst, int x, int y, int width, u16 height) {
  int col;
  u16 mask0;
  int startY;
  u16* dot;
  u32 slot;
  int rowOff;
  u16* cell;
  u16* column;
  u16 mask1;
  u16 mask2;
  u16 mask3;
  int endCol;
  u16* rowTable;

  /* Read through a local so the row offset is reloaded for each tile, as
     * in MapPreview_DrawBlock. */
  rowTable = gMapPreviewRowOffsets;
  startY = y;
  if (y >= startY + height) {
    return;
  }
  endCol = x + width;
  mask0 = gMapPreviewBlockUnitMask[0];
  mask1 = gMapPreviewBlockUnitMask[1];
  mask2 = gMapPreviewBlockUnitMask[2];
  mask3 = gMapPreviewBlockUnitMask[3];
  do {
    for (col = x; col < endCol; col++) {
      column = dst + gMapPreviewColumnOffsets[col - x];
      rowOff = rowTable[y - startY];
      cell = column + rowOff;
      column[rowOff] = cell[2] = cell[4] = cell[6] = 0;
      slot = GAME->mapTiles->unitAt[GAME->mapTiles->rowStart[y] + col];
      if (slot != 0) {
        /* unit slot / 64 + 1: the owner's player slot */
        dot = data_02139d90 +
              (data_021587e0 + 18)[(gSavedArmyColours + 1)[slot >> 6]];
        cell[0] &= mask0;
        cell[2] &= mask1;
        cell[4] &= mask2;
        cell[6] &= mask3;
        cell[0] |= dot[0];
        cell[2] |= dot[2];
        cell[4] |= dot[4];
        cell[6] |= dot[6];
      }
    }
    y++;
  } while (y < startY + height);
}
