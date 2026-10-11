#include "sdk/fx.h"

/* dst = v2 + a * v1, each component in fx32 (no rounding). */
void VEC_MultAdd(fx32 a, const VecFx32 *v1, const VecFx32 *v2, VecFx32 *dst)
{
    dst->x = v2->x + (fx32)(((long long)a * v1->x) >> 12);
    dst->y = v2->y + (fx32)(((long long)a * v1->y) >> 12);
    dst->z = v2->z + (fx32)(((long long)a * v1->z) >> 12);
}
