#include "types.h"

extern u16 data_02158748[2];
extern u16 data_0215874c[2];
extern u16 data_02158754[2];
extern u16 data_02158758[2];

/* Writes a middle row of a window frame into a tile map row: a left edge
 * tile, width - 3 inner tiles and two right edge tiles. rowKind (0 or 1)
 * picks the tile from each of the four tables, so alternating kinds stripe
 * the frame. Tiles outside columns 0 to 31 are skipped. */
void Window_DrawFrameMiddleRow(u16* row, int x, s16 width, int rowKind,
                               int palette) {
  u16 i;
  int middle;
  u16 tileBits = palette << 12;

  if (x >= 0 && x < 32) {
    *row = tileBits | data_02158748[rowKind];
  }
  middle = width - 3;
  row++;
  x++;
  for (i = 0; i < middle; i++) {
    if (x >= 0 && x < 32) {
      *row = tileBits | data_0215874c[rowKind];
    }
    row++;
    x++;
  }
  if (x >= 0 && x < 32) {
    *row = tileBits | data_02158758[rowKind];
  }
  x++;
  if (x < 0) {
    return;
  }
  if (x < 32) {
    row[1] = tileBits | data_02158754[rowKind];
  }
}
