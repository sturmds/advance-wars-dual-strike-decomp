#include "types.h"
#include "sdk/msl.h"

FpValue __fp_trap(FpValue x, FpValue y, u32 op);

s32 func_02121d58(double a, double b, u32 op) {
  FpValue x;
  FpValue y;
  x.d = a;
  y.d = b;
  return __fp_trap(x, y, op).i;
}
