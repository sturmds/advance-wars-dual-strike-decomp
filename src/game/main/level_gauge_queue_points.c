#include "types.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void func_02049ddc(int x, int y, int digit, int matrixIndex, int frame);

/* Queues a gauge's points (0-999) right-aligned in three digit columns,
 * each digit 4 frames behind the one to its left. */
void LevelGauge_QueuePoints(int x, int y, int value, int matrixIndex,
                            int frame) {
  int hundreds;
  int tens;
  int ones;
  int rest;

  if (value >= 100) {
    hundreds = HwDiv32(value, 100);
    rest = HwMod32(value, 100);
    func_02049ddc(x, y, hundreds, matrixIndex, frame);
    tens = HwDiv32(rest, 10);
    ones = HwMod32(rest, 10);
    func_02049ddc(x + 8, y, tens, matrixIndex + 1, frame - 4);
    func_02049ddc(x + 16, y, ones, matrixIndex + 2, frame - 8);
  } else if (value >= 10) {
    tens = HwDiv32(value, 10);
    ones = HwMod32(value, 10);
    func_02049ddc(x + 8, y, tens, matrixIndex, frame);
    func_02049ddc(x + 16, y, ones, matrixIndex + 1, frame - 4);
  } else {
    func_02049ddc(x + 16, y, value, matrixIndex, frame);
  }
}
