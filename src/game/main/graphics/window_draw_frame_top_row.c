#include "types.h"

/* Writes the top row of a window frame into a tile map row: a corner tile, a
 * run of edge tiles and a corner tile. Tiles outside columns 0 to 31 are
 * skipped. x is the column of the first tile. */
void Window_DrawFrameTopRow(u16* row, int x, s16 width, int palette) {
  u16 i;
  u32 tileBits = (u32)(palette << 28) >> 16;
  int middle;

  if (x >= 0 && x < 32) {
    *row = tileBits | 1;
  }
  middle = width - 2;
  row++;
  x++;
  for (i = 0; i < middle; i++) {
    if (x >= 0 && x < 32) {
      *row = tileBits | 2;
    }
    row++;
    x++;
  }
  if (x < 0) {
    return;
  }
  if (x < 32) {
    *row = tileBits | 3;
  }
}
