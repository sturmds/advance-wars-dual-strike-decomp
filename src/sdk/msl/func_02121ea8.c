#include "types.h"
#include "sdk/msl.h"

FpValue func_02121f3c(FpValue x, FpValue y, u32 op);

s32 func_02121ea8(double a, u32 op)
{
    FpValue x;
    x.d = a;
    return func_02121f3c(x, x, op).i;
}
