#include "types.h"
#include "game/math.h"

void NumberSprite_PutGlyph(int x, int y, int glyph, int firstTile, int palette);

/* Draws value as two digits (value / 10 is not reduced, so keep it below 100):
 * ones at x, tens 15 pixels left. */
void NumberSprite_PutTwoDigits(int x, int y, int value, int firstTile,
                               int palette) {
  int tens;
  int ones;

  ones = HwMod32(value, 10);
  tens = HwDiv32(value, 10);
  NumberSprite_PutGlyph(x, y, ones, firstTile, palette);
  NumberSprite_PutGlyph(x - 15, y, tens, firstTile, palette);
}
