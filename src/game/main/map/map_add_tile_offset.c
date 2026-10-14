#include "types.h"
#include "game/tile_offset.h"

/* Adds entry `index` of offset list `list` to x and y. Returns FALSE at the
 * end of the list, marked by a dx of TILE_OFFSET_END. */
BOOL Map_AddTileOffset(int* x, int* y, int list, int index) {
  TileOffset* offsets = data_0216638c[list];

  if (offsets[index].dx == TILE_OFFSET_END) {
    return FALSE;
  }
  *x += offsets[index].dx;
  *y += offsets[index].dy;
  return TRUE;
}
