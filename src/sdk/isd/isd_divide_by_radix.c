/* Needed to match: the library was built optimizing for size, which keeps the correction loop's product recomputed. */
#pragma optimize_for_size on
#include "types.h"

/* Returns value / radix for radix 2, 8, 10 and 16, and 0 for any other. */
u32 Isd_DivideByRadix(u32 value, u32 radix)
{
    u32 low;
    u32 high;
    u32 quotient;

    switch (radix) {
    case 2:
        return value >> 1;
    case 8:
        return value >> 3;
    case 16:
        return value >> 4;
    case 10:
        low = value & 0xffff;
        high = value >> 16;
        quotient = high * 0x1999 + ((high * 0x9999 + low * 0x1999) >> 16);
        if (value >= 10) {
            while (quotient * 10 < value - 9) {
                quotient++;
            }
        }
        return quotient;
    default:
        return 0;
    }
}
