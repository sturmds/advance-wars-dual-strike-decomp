#include "types.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* One digit in the same sprites as WinLoseScreen_DrawPointsDigit, drawn plain. */
#define DRAW_DIGIT(x, y, digit)                              \
  Sprite_QueueAt(6, (x) & 0x1ff, (y) & 0xff, &data_027c0040, \
                 ((digit) * 4 + 0xf8) | 0x6000, 0)

/* Draws `value` right-aligned as five 12-pixel digit places from x, without
 * leading zeros and without the pop-in of WinLoseScreen_DrawPoints. A
 * negative value draws nothing. The original jumps into the next place once
 * a digit is drawn; the gotos reproduce that branch shape. */
void WinLoseScreen_DrawNumber(int x, int y, int value) {
  if (value >= 10000) {
    DRAW_DIGIT(x, y, HwDiv32(value, 10000));
    value = HwMod32(value, 10000);
    goto thousands;
  }
  if (value >= 1000) {
  thousands:
    DRAW_DIGIT(x + 12, y, HwDiv32(value, 1000));
    value = HwMod32(value, 1000);
    goto hundreds;
  }
  if (value >= 100) {
  hundreds:
    DRAW_DIGIT(x + 24, y, HwDiv32(value, 100));
    value = HwMod32(value, 100);
    goto tens;
  }
  if (value >= 10) {
  tens:
    DRAW_DIGIT(x + 36, y, HwDiv32(value, 10));
    value = HwMod32(value, 10);
    goto ones;
  }
  if (value >= 0) {
  ones:
    DRAW_DIGIT(x + 48, y, value);
  }
}
