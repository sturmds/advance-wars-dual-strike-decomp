#include "sdk/gx.h"
#include "sdk/math.h"
#include "sdk/wmsp.h"

/* Needed to match: the divide by 20 is inlined, as -O4,p builds it. */
#pragma optimize_for_size off

u16 func_arm7_027c6020(u32 parentSize, u32 childSize, u32 childBitmap)
{
    u32 needed = (parentSize + 34) * 4 + 96 + (childSize * MATH_CountPopulation_Arm7(childBitmap) + 904);
    int lines = 258 - *REG_VCOUNT;
    int time;

    if (lines < 0) {
        lines += 263;
    }
    time = lines * 127 / 20;
    return time * 10 < needed ? 0 : time;
}
