#include "types.h"
#include "sdk/msl.h"

FpValue func_02121f3c(FpValue x, FpValue y, u32 op);

double func_02121e58(double a, u32 op)
{
    FpValue x;
    x.d = a;
    return func_02121f3c(x, x, op).d;
}
