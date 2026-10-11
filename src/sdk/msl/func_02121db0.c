#include "types.h"
#include "sdk/msl.h"

FpValue func_02121f3c(FpValue x, FpValue y, u32 op);

double func_02121db0(double a, double b, u32 op)
{
    FpValue x;
    FpValue y;
    x.d = a;
    y.d = b;
    return func_02121f3c(x, y, op).d;
}
