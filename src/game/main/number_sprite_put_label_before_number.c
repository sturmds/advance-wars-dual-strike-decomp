#include "types.h"
#include "game/screen_other_map.h"

void NumberSprite_PutGlyph(int x, int y, int glyph, int firstTile, int palette);
void NumberSprite_PutNumber(int x, int y, int value, int firstTile,
                            int palette);

/* On the second screen, draws value (at least 1) with the two label glyphs 11
 * and 12 in front of it, the digits starting at x + 57. */
void NumberSprite_PutLabelBeforeNumber(int x, int y, int value, int firstTile,
                                       int palette) {
  Unk_027c0280* saved0280;

  if (value <= 0) {
    value = 1;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  NumberSprite_PutNumber(x + 57, y, value, firstTile, palette);
  x += 51;
  if (value >= 10) {
    x -= 15;
  }
  if (value >= 100) {
    x -= 15;
  }
  NumberSprite_PutGlyph(x, y, 12, firstTile, palette);
  NumberSprite_PutGlyph(x - 16, y, 11, firstTile, palette);
  Screen_SetCurrentGfx(saved0280);
}
