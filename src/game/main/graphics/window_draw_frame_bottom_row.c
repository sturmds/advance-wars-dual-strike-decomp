#include "types.h"

extern u16 gWindowFrameBottomTiles[];

/* Writes the bottom row of a window frame into a tile map row: a corner tile,
 * edge tiles that alternate between the two entries of gWindowFrameBottomTiles
 * and a corner tile. Tiles outside columns 0 to 31 are skipped. */
void Window_DrawFrameBottomRow(u16* row, int x, s16 width, int palette) {
  u16 edge[2];
  u32 tileBits;
  u16 i;
  int middle;

  edge[0] = gWindowFrameBottomTiles[0];
  edge[1] = gWindowFrameBottomTiles[1];
  tileBits = (u32)(palette << 28) >> 16;
  if (x >= 0 && x < 32) {
    *row = tileBits | 7;
  }
  middle = width - 2;
  row++;
  x++;
  for (i = 0; i < middle; i++) {
    if (x >= 0 && x < 32) {
      *row = tileBits | edge[i % 2];
    }
    row++;
    x++;
  }
  if (x < 0) {
    return;
  }
  if (x < 32) {
    *row = tileBits | 9;
  }
}
