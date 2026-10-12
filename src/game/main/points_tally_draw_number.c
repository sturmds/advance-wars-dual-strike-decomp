#include "types.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void PointsTally_DrawDigit(int x, int y, int digit, int palette, int matrix,
                           int popFrame);

/* Draws value right-aligned in four 12-pixel digit cells from x, without
 * leading zeros. Each digit pops in 4 frames after the one before it. */
void PointsTally_DrawNumber(int x, int y, int value, int palette,
                            int popFrame) {
  int digit;
  int rest;

  if (value >= 1000) {
    digit = HwDiv32(value, 1000);
    rest = HwMod32(value, 1000);
    PointsTally_DrawDigit(x, y, digit, palette, 1, popFrame);
    digit = HwDiv32(rest, 100);
    rest = HwMod32(rest, 100);
    PointsTally_DrawDigit(x + 12, y, digit, palette, 2, popFrame - 4);
    digit = HwDiv32(rest, 10);
    rest = HwMod32(rest, 10);
    PointsTally_DrawDigit(x + 24, y, digit, palette, 3, popFrame - 8);
    PointsTally_DrawDigit(x + 36, y, rest, palette, 4, popFrame - 12);
  } else if (value >= 100) {
    digit = HwDiv32(value, 100);
    rest = HwMod32(value, 100);
    PointsTally_DrawDigit(x + 12, y, digit, palette, 1, popFrame);
    digit = HwDiv32(rest, 10);
    rest = HwMod32(rest, 10);
    PointsTally_DrawDigit(x + 24, y, digit, palette, 2, popFrame - 4);
    PointsTally_DrawDigit(x + 36, y, rest, palette, 3, popFrame - 8);
  } else if (value >= 10) {
    digit = HwDiv32(value, 10);
    rest = HwMod32(value, 10);
    PointsTally_DrawDigit(x + 24, y, digit, palette, 1, popFrame);
    PointsTally_DrawDigit(x + 36, y, rest, palette, 2, popFrame - 4);
  } else {
    PointsTally_DrawDigit(x + 36, y, value, palette, 1, popFrame);
  }
}
