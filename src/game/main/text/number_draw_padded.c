#include "types.h"

/* Draws value as a two-tile-tall decimal number on a BG screen map, padded
 * with zeros to digits digits. (x, y) is the top row of the last digit and the
 * number grows leftwards. Each digit set has 20 tiles from tile 54: a top and
 * a bottom tile per digit. entryBits is ORed into every map entry (palette). */
void Number_DrawPadded(int x, int y, u16* map, u32 value, u16 entryBits,
                       u16 digitSet, int digits) {
  u16* cell = map + x + y * 32;

  while (digits-- != 0) {
    u32 tile = 54 + digitSet * 20 + (value % 10) * 2;
    cell[0] = entryBits | tile;
    cell[32] = entryBits | (tile + 1);
    cell--;
    value /= 10;
  }
}
