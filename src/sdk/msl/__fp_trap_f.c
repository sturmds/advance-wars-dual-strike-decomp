#include "types.h"
#include "sdk/msl.h"

FpValue __fp_trap(FpValue x, FpValue y, u32 op);

float __fp_trap_f(float a, u32 op)
{
    FpValue v;
    v.f = a;
    return __fp_trap(v, v, op).f;
}
