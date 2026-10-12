#include "types.h"

int HwMod32(int numerator, int denominator);
void NumberSprite_PutGlyph(int x, int y, int glyph, int firstTile, int palette);

/* Draws value in decimal, last digit at x and each earlier digit 15 pixels
 * further left. */
void NumberSprite_PutNumber(int x, int y, int value, int firstTile,
                            int palette) {
  do {
    NumberSprite_PutGlyph(x, y, HwMod32(value, 10), firstTile, palette);
    value /= 10;
    x -= 15;
  } while (value != 0);
}
