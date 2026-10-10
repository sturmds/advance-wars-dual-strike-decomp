#include "types.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void func_0204a00c(int x, int y, int matrixIndex, int frame);
void func_0204a184(int x, int y, int digit, int matrixIndex, int frame);

void func_02049f68(int x, int y, int value, int matrixIndex, int frame)
{
    int tens;

    if (value == 100) {
        func_0204a00c(x, y, matrixIndex, frame);
        return;
    }
    if (value >= 10) {
        tens = HwDiv32(value, 10);
        value = HwMod32(value, 10);
        func_0204a184(x, y, tens, matrixIndex, frame);
    }
    func_0204a184(x + 8, y, value, matrixIndex + 1, frame);
}
