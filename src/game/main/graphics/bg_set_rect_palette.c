#include "types.h"

/* Sets the palette of every entry in a width x height rectangle at (x, y) of a
 * 32-tile-wide tile map. */
void Bg_SetRectPalette(u16* map, int x, int y, int width, u16 height,
                       u16 palette) {
  int row;
  int col;

  for (row = y; row < y + height; row++) {
    for (col = x; col < x + width; col++) {
      map[col + row * 32] &= 0xfff;
      map[col + row * 32] |= palette << 12;
    }
  }
}
