#include "types.h"

typedef struct TileOffset {
  s16 dx;
  s16 dy;
} TileOffset;

extern TileOffset* data_0216638c[];

/* Adds entry `index` of offset list `list` to x and y. Returns FALSE at the
 * end of the list, marked by a dx of 127. */
BOOL Map_AddTileOffset(int* x, int* y, int list, int index) {
  TileOffset* offsets = data_0216638c[list];

  if (offsets[index].dx == 127) {
    return FALSE;
  }
  *x += offsets[index].dx;
  *y += offsets[index].dy;
  return TRUE;
}
