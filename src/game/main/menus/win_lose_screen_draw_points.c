#include "types.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void func_0203f854(int x, int y, int digit, int order, int frame);

/* Draws `value` (capped at 99999) right-aligned as five 12-pixel digit
 * places from x, without leading zeros. func_0203f854 draws one digit with
 * its order from the left (1 first) and a frame count; each digit after the
 * first gets a count 4 frames lower, so the digits appear one after another. */
void WinLoseScreen_DrawPoints(int x, int y, int value, int frame)
{
    int digit;
    int rest;

    if (value > 99999) {
        value = 99999;
    }
    if (value >= 10000) {
        digit = HwDiv32(value, 10000);
        rest = HwMod32(value, 10000);
        func_0203f854(x, y, digit, 1, frame);
        digit = HwDiv32(rest, 1000);
        rest = HwMod32(rest, 1000);
        func_0203f854(x + 12, y, digit, 2, frame - 4);
        digit = HwDiv32(rest, 100);
        rest = HwMod32(rest, 100);
        func_0203f854(x + 24, y, digit, 3, frame - 8);
        digit = HwDiv32(rest, 10);
        rest = HwMod32(rest, 10);
        func_0203f854(x + 36, y, digit, 4, frame - 12);
        func_0203f854(x + 48, y, rest, 5, frame - 16);
    } else if (value >= 1000) {
        digit = HwDiv32(value, 1000);
        rest = HwMod32(value, 1000);
        func_0203f854(x + 12, y, digit, 1, frame);
        digit = HwDiv32(rest, 100);
        rest = HwMod32(rest, 100);
        func_0203f854(x + 24, y, digit, 2, frame - 4);
        digit = HwDiv32(rest, 10);
        rest = HwMod32(rest, 10);
        func_0203f854(x + 36, y, digit, 3, frame - 8);
        func_0203f854(x + 48, y, rest, 4, frame - 12);
    } else if (value >= 100) {
        digit = HwDiv32(value, 100);
        rest = HwMod32(value, 100);
        func_0203f854(x + 24, y, digit, 1, frame);
        digit = HwDiv32(rest, 10);
        rest = HwMod32(rest, 10);
        func_0203f854(x + 36, y, digit, 2, frame - 4);
        func_0203f854(x + 48, y, rest, 3, frame - 8);
    } else if (value >= 10) {
        digit = HwDiv32(value, 10);
        rest = HwMod32(value, 10);
        func_0203f854(x + 36, y, digit, 1, frame);
        func_0203f854(x + 48, y, rest, 2, frame - 4);
    } else {
        func_0203f854(x + 48, y, value, 1, frame);
    }
}
