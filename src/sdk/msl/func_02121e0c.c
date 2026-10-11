#include "types.h"
#include "sdk/msl.h"

FpValue func_02121f3c(FpValue x, FpValue y, u32 op);

float func_02121e0c(float a, float b, u32 op)
{
    FpValue x;
    FpValue y;
    x.f = a;
    y.f = b;
    return func_02121f3c(x, y, op).f;
}
