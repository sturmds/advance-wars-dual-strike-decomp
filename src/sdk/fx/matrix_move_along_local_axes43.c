#include "sdk/fx.h"

void MI_Copy36B(MtxFx43 *src, MtxFx43 *dest);

/* Copies src to dest, then adds (x, y, z), turned by src's 3x3 part, to
 * the translation row. MI_Copy36B copies the 36 bytes of the 3x3 part. */
void Matrix_MoveAlongLocalAxes43(MtxFx43 *src, MtxFx43 *dest, fx32 x, fx32 y, fx32 z)
{
    if (src != dest) {
        MI_Copy36B(src, dest);
    }
    dest->m[3][0] = src->m[3][0] + (fx32)(((long long)x * src->m[0][0] + (long long)y * src->m[1][0] + (long long)z * src->m[2][0]) >> 12);
    dest->m[3][1] = src->m[3][1] + (fx32)(((long long)x * src->m[0][1] + (long long)y * src->m[1][1] + (long long)z * src->m[2][1]) >> 12);
    dest->m[3][2] = src->m[3][2] + (fx32)(((long long)x * src->m[0][2] + (long long)y * src->m[1][2] + (long long)z * src->m[2][2]) >> 12);
}
