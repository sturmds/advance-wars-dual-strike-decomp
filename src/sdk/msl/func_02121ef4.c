#include "types.h"
#include "sdk/msl.h"

FpValue func_02121f3c(FpValue x, FpValue y, u32 op);

float func_02121ef4(float a, u32 op)
{
    FpValue v;
    v.f = a;
    return func_02121f3c(v, v, op).f;
}
