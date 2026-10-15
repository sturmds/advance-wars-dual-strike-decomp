#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "types.h"

extern const s16 gSineTable[];
s32 HwDiv32(s32 numerator, s32 denominator);
u32 HwDivU32(u32 numer, u32 denom);
s32 HwMod32(s32 numer, s32 denom);
u32 HwModU32(u32 numer, u32 denom);
u32 Math_Sqrt32(u32 value);
int Math_Distance(int x1, int y1, int x2, int y2);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
int Math_EaseInOut(int from, int to, int elapsed, int duration);
int Math_EaseOvershoot(int amount, int from, int to, int elapsed, int duration);
s16 Math_SinDegrees(int degrees);

#endif
