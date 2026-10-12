#include "types.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void func_020344dc(int x, int y, int matrixIndex, int frame);
void func_02034654(int x, int y, int digit, int matrixIndex, int frame);

/* Queues a number of up to 100 as pop-in digit sprites. 100 is drawn by one
 * call; below that the digits are right-aligned, 8 pixels apart, and the ones
 * digit always uses matrix matrixIndex + 1. */
void DigitSprite_QueueSmallNumber(int x, int y, int value, int matrixIndex,
                                  int frame) {
  int tens;

  if (value == 100) {
    func_020344dc(x, y, matrixIndex, frame);
    return;
  }
  if (value >= 10) {
    tens = HwDiv32(value, 10);
    value = HwMod32(value, 10);
    func_02034654(x, y, tens, matrixIndex, frame);
  }
  func_02034654(x + 8, y, value, matrixIndex + 1, frame);
}
