#include "types.h"

/* Draws one empty skill slot: the 2x2 tiles 21-24 at (x, y) of a
 * 32-tile-wide tile map, with the given palette. */
void Bg_PutSkillSlot(u16 x, u16 y, u16* map, int palette) {
  map[x + y * 32] = (palette << 12) | 21;
  map[x + 1 + y * 32] = (palette << 12) | 22;
  map[x + (y + 1) * 32] = (palette << 12) | 23;
  map[x + 1 + (y + 1) * 32] = (palette << 12) | 24;
}
