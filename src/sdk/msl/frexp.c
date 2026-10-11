#include "types.h"
#include "sdk/msl.h"

double func_02126064(double a, double b);

double frexp(double x, int *exponent)
{
    int high = ((int *)&x)[1];
    int highAbs = high & 0x7FFFFFFF;
    u32 low = ((u32 *)&x)[0];

    *exponent = 0;
    if (highAbs >= 0x7FF00000 || (highAbs | low) == 0) {
        return x;
    }
    if (highAbs < 0x00100000) {
        /* Subnormal: scale by 2^54 (func_02126064 is the double multiply) and note it in the exponent. */
        x = func_02126064(x, 18014398509481984.0);
        high = ((int *)&x)[1];
        highAbs = high & 0x7FFFFFFF;
        *exponent = -54;
    }
    *exponent += (highAbs >> 20) - 1022;
    high = (high & 0x800FFFFF) | 0x3FE00000;
    ((int *)&x)[1] = high;
    return x;
}
