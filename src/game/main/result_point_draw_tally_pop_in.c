#include "types.h"
#include "game/Unk_0229c734.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void ResultPoint_QueueDigitPopIn(int x, int y, int digit, int matrixIndex,
                                 int frame, int palette);
void ResultPoint_QueueNumber(int x, int y, int value, int palette);

#define SCORES data_0229c734

/* Draws a number of up to four digits in palette 1, x being the left of a
 * three digit number. Each digit pops in 4 frames after the one before,
 * through the affine matrices after those of the score rows on the page
 * (pageRowCount rows, three each). A negative frame draws it plain. */
void ResultPoint_DrawTallyPopIn(int x, int y, int value, int frame) {
  int matrixIndex;
  int digit;
  int rest;

  if (frame < 0) {
    ResultPoint_QueueNumber(x, y, value, 1);
    return;
  }
  matrixIndex = SCORES->pageRowCount * 3;
  if (value >= 1000) {
    digit = HwDiv32(value, 1000);
    rest = HwMod32(value, 1000);
    ResultPoint_QueueDigitPopIn(x - 12, y, digit, matrixIndex, frame, 1);
    digit = HwDiv32(rest, 100);
    rest = HwMod32(rest, 100);
    ResultPoint_QueueDigitPopIn(x, y, digit, matrixIndex + 1, frame - 4, 1);
    digit = HwDiv32(rest, 10);
    rest = HwMod32(rest, 10);
    ResultPoint_QueueDigitPopIn(x + 12, y, digit, matrixIndex + 2, frame - 8,
                                1);
    ResultPoint_QueueDigitPopIn(x + 24, y, rest, matrixIndex + 3, frame - 12,
                                1);
  } else if (value >= 100) {
    digit = HwDiv32(value, 100);
    rest = HwMod32(value, 100);
    ResultPoint_QueueDigitPopIn(x, y, digit, matrixIndex, frame, 1);
    digit = HwDiv32(rest, 10);
    rest = HwMod32(rest, 10);
    ResultPoint_QueueDigitPopIn(x + 12, y, digit, matrixIndex + 1, frame - 4,
                                1);
    ResultPoint_QueueDigitPopIn(x + 24, y, rest, matrixIndex + 2, frame - 8, 1);
  } else if (value >= 10) {
    digit = HwDiv32(value, 10);
    rest = HwMod32(value, 10);
    ResultPoint_QueueDigitPopIn(x + 12, y, digit, matrixIndex, frame, 1);
    ResultPoint_QueueDigitPopIn(x + 24, y, rest, matrixIndex + 1, frame - 4, 1);
  } else {
    ResultPoint_QueueDigitPopIn(x + 24, y, value, matrixIndex, frame, 1);
  }
}
