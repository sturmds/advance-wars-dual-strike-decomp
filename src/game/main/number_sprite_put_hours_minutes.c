#include "types.h"
#include "game/screen_other_map.h"

int HwDiv32(int numerator, int denominator);
int HwMod32(int numerator, int denominator);
void NumberSprite_PutGlyph(int x, int y, int glyph, int firstTile, int palette);
void NumberSprite_PutTwoDigits(int x, int y, int value, int firstTile,
                               int palette);

/* On the second screen, draws a time given in seconds as hours, a colon
 * (glyph 10) and minutes, two digits each. */
void NumberSprite_PutHoursMinutes(int x, int y, int seconds, int firstTile,
                                  int palette) {
  int hours;
  int minutes;
  int totalMinutes;
  Unk_027c0280* saved0280;

  totalMinutes = HwDiv32(seconds, 60);
  minutes = HwMod32(totalMinutes, 60);
  hours = HwDiv32(totalMinutes, 60);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  NumberSprite_PutTwoDigits(x + 60, y, minutes, firstTile, palette);
  NumberSprite_PutGlyph(x + 30, y, 10, firstTile, palette);
  NumberSprite_PutTwoDigits(x + 15, y, hours, firstTile, palette);
  Screen_SetCurrentGfx(saved0280);
}
