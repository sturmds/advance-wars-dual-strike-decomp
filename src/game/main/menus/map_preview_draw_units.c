#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gSavedArmyColours[];
extern u16 gMapPreviewColumnOffsets[];
extern u16 gMapPreviewRowOffsets[];
extern u16 data_021587e0[];
extern u16 data_02139d90[];
/* One u16 (four 4bpp pixels) per row of a 4x4 preview cell: 0xf00f, 0, 0,
 * 0xf00f, so a unit dot keeps the cell's corners. */
extern u16 gMapPreviewUnitMask[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Draws a dot in the owner's army colour on every map preview cell that has
 * a unit (MapPreviewTask_Update runs it after the terrain steps). The dot
 * pictures come from data_02139d90, picked through data_021587e0 from entry
 * 18 on, as MapPreview_DrawLowerHalf picks terrain pictures. */
void MapPreview_DrawUnits(u16* dst) {
  int col;
  int row;
  u32 slot;
  u16* dot;
  u16* cell;
  u16 mask0;
  u16 mask1;
  u16 mask2;
  u16 mask3;

  /* Needed to match: an early return and a do-while, so the masks load once
     * between the height check and the first row. */
  row = 0;
  if (row >= GAME->unk_1c4->mapHeight) {
    return;
  }
  mask0 = gMapPreviewUnitMask[0];
  mask1 = gMapPreviewUnitMask[1];
  mask2 = gMapPreviewUnitMask[2];
  mask3 = gMapPreviewUnitMask[3];
  do {
    for (col = 0; col < GAME->unk_1c4->mapWidth; col++) {
      slot = GAME->mapTiles->unitAt[GAME->mapTiles->rowStart[row] + col];
      if (slot != 0) {
        /* unit slot / 64 + 1: the owner's player slot */
        dot = data_02139d90 +
              (data_021587e0 + 18)[(gSavedArmyColours + 1)[slot >> 6]];
        cell = dst + gMapPreviewColumnOffsets[col] + gMapPreviewRowOffsets[row];
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
    row++;
  } while (row < GAME->unk_1c4->mapHeight);
}
