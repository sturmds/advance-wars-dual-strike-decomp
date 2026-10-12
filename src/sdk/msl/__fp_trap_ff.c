#include "types.h"
#include "sdk/msl.h"

FpValue __fp_trap(FpValue x, FpValue y, u32 op);

float __fp_trap_ff(float a, float b, u32 op) {
  FpValue x;
  FpValue y;
  x.f = a;
  y.f = b;
  return __fp_trap(x, y, op).f;
}
