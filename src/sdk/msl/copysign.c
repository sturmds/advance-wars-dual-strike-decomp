#include "types.h"
#include "sdk/msl.h"

double copysign(double x, double y)
{
    ((u32 *)&x)[1] = (((u32 *)&x)[1] & 0x7FFFFFFF) | (((u32 *)&y)[1] & 0x80000000);
    return x;
}
