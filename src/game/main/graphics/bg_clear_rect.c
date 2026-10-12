#include "types.h"

/* Clears a width x height rectangle at (x, y) of a 32-tile-wide tile map
 * to tile 0. */
void Bg_ClearRect(u16* map, int x, int y, int width, u16 height) {
  int row;
  int col;

  for (row = y; row < y + height; row++) {
    for (col = x; col < x + width; col++) {
      map[col + row * 32] = 0;
    }
  }
}
