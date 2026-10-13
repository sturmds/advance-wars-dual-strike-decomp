#include "types.h"
#include "sdk/fp_status.h"
#include "sdk/msl.h"

FpValue __fp_trap(FpValue x, FpValue y, u32 op);

s32 FpTrap_DoubleToInt(double a, u32 op) {
  FpValue x;
  x.d = a;
  return __fp_trap(x, x, op).i;
}
