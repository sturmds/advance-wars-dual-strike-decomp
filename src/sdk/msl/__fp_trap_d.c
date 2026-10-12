#include "types.h"
#include "sdk/msl.h"

FpValue __fp_trap(FpValue x, FpValue y, u32 op);

double __fp_trap_d(double a, u32 op) {
  FpValue x;
  x.d = a;
  return __fp_trap(x, x, op).d;
}
