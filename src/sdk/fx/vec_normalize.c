#include "sdk/fx.h"
#include "hw_sqrt.h"

/* dst = src scaled to length 1.0. With t the squared
 * length, the divider gives 2^56 / t while the square root unit gives
 * sqrt(4t) = 2 |src|; their product is 2^57 / |src|, and each coordinate times
 * that, rounded, >> 45 is the coordinate * 1.0 / |src|. */
void VEC_Normalize(const VecFx32* src, VecFx32* dst) {
  fx64 t;
  fx32 sqrt;

  t = (fx64)src->x * src->x;
  t += (fx64)src->y * src->y;
  t += (fx64)src->z * src->z;

  REG_DIVCNT = DIVCNT_MODE_64_64;
  IO_DIVIDER_DIVIDEND = (fx64)1 << 56;
  REG_DIV_DENOM = t;
  IO_ROOT_UNIT_CTRL = SQRTCON_MODE_64;
  IO_ROOT_UNIT_INPUT = t * 4;
  while (IO_ROOT_UNIT_CTRL & SQRTCON_BUSY) {
  }
  sqrt = REG_SQRT_RESULT;
  while (REG_DIVCNT & DIVCNT_BUSY) {
  }
  t = IO_DIVIDER_QUOTIENT * sqrt;

  dst->x = (fx32)((t * src->x + ((fx64)1 << 44)) >> 45);
  dst->y = (fx32)((t * src->y + ((fx64)1 << 44)) >> 45);
  dst->z = (fx32)((t * src->z + ((fx64)1 << 44)) >> 45);
}
