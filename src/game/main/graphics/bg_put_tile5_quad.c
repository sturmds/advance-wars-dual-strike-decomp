#include "types.h"

/* Fills the 2x2 block at (x, y) of a 32-tile-wide tile map with tile 5 in the
 * given palette. */
void Bg_PutTile5Quad(u16 x, u16 y, u16* map, int palette) {
  u16 entry = (palette << 12) | 5;

  map[x + y * 32] = entry;
  map[x + 1 + y * 32] = entry;
  map[x + (y + 1) * 32] = entry;
  map[x + 1 + (y + 1) * 32] = entry;
}
