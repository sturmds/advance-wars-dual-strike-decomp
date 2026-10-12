#include "sdk/fx.h"

/* a . b in fx32, rounded. */
fx32 VEC_DotProduct(const VecFx32* a, const VecFx32* b) {
  return (fx32)(((long long)a->x * b->x + (long long)a->y * b->y +
                 (long long)a->z * b->z + 0x800) >>
                12);
}
