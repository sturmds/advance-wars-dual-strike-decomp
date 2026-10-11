#include "sdk/fx.h"

/* dst = a x b, each component rounded to fx32. Needed to match: the
 * components of a are read into locals first, those of b are not. */
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *dst)
{
    fx32 ay = a->y;
    fx32 ax = a->x;
    fx32 az = a->z;
    long long x = (long long)ay * b->z - (long long)az * b->y;
    long long y = (long long)az * b->x - (long long)ax * b->z;
    long long z = (long long)ax * b->y - (long long)ay * b->x;

    dst->x = (fx32)((x + 0x800) >> 12);
    dst->y = (fx32)((y + 0x800) >> 12);
    dst->z = (fx32)((z + 0x800) >> 12);
}
