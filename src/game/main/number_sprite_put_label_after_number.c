#include "types.h"
#include "game/screen_other_map.h"

void NumberSprite_PutGlyph(int x, int y, int glyph, int firstTile, int palette);
void NumberSprite_PutNumber(int x, int y, int value, int firstTile,
                            int palette);

/* On the second screen, draws value ending at x + 37 with label glyph 11 and
 * then glyph 12 after it (13 when value is above 1). */
void NumberSprite_PutLabelAfterNumber(int x, int y, int value, int firstTile,
                                      int palette) {
  Unk_027c0280* saved0280 = data_027c0280;
  int unitGlyph;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  unitGlyph = 12;
  if (value > 1) {
    unitGlyph = 13;
  }
  NumberSprite_PutGlyph(x + 68, y, unitGlyph, firstTile, palette);
  NumberSprite_PutGlyph(x + 52, y, 11, firstTile, palette);
  NumberSprite_PutNumber(x + 37, y, value, firstTile, palette);
  Screen_SetCurrentGfx(saved0280);
}
