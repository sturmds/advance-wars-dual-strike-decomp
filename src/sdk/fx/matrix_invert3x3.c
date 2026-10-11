#include "sdk/fx.h"

void MI_Copy36B(const void *src, void *dst); /* MI_Copy36B */

/* Matrix_Invert3x3: writes the inverse of src to dst (they may be the
 * same matrix). Returns -1 and leaves dst alone when src has no inverse.
 * det0 to det2 are the cofactors of the first row; the reciprocal of the
 * determinant runs on the hardware divider while the other cofactors are
 * worked out.
 * Not matched yet: 6 instructions in the first cofactors differ (register
 * order and scheduling). Swapping the factors of the second products of
 * det1 and det2 came closest. */
int Matrix_Invert3x3(const MtxFx33 *src, MtxFx33 *dst)
{
    MtxFx33 tmp;
    MtxFx33 *out;
    fx32 det;
    fx64 det0, det1, det2;
    fx32 var0, var1, var2, var3;

    out = (src == dst) ? &tmp : dst;

    det1 = (fx32)(((fx64)src->m[1][0] * src->m[2][2] - (fx64)src->m[2][0] * src->m[1][2] + FX32_HALF) >> FX32_SHIFT);
    det0 = (fx32)(((fx64)src->m[1][1] * src->m[2][2] - (fx64)src->m[1][2] * src->m[2][1] + FX32_HALF) >> FX32_SHIFT);
    det2 = (fx32)(((fx64)src->m[1][0] * src->m[2][1] - (fx64)src->m[2][0] * src->m[1][1] + FX32_HALF) >> FX32_SHIFT);
    det = (fx32)(((fx64)src->m[0][0] * det0 - (fx64)src->m[0][1] * det1 + (fx64)src->m[0][2] * det2 + FX32_HALF) >> FX32_SHIFT);
    if (det == 0) {
        return -1;
    }
    FX_InvAsync(det);

    var0 = (fx32)(((fx64)src->m[0][1] * src->m[2][2] - (fx64)src->m[2][1] * src->m[0][2]) >> FX32_SHIFT);
    var1 = (fx32)(((fx64)src->m[0][1] * src->m[1][2] - (fx64)src->m[1][1] * src->m[0][2]) >> FX32_SHIFT);
    var2 = (fx32)(((fx64)src->m[0][0] * src->m[2][2] - (fx64)src->m[2][0] * src->m[0][2]) >> FX32_SHIFT);
    var3 = (fx32)(((fx64)src->m[0][0] * src->m[1][2] - (fx64)src->m[1][0] * src->m[0][2]) >> FX32_SHIFT);

    det = FX_GetDivResult();

    out->m[0][0] = (fx32)(((fx64)det * det0) >> FX32_SHIFT);
    out->m[0][1] = -(fx32)(((fx64)det * var0) >> FX32_SHIFT);
    out->m[0][2] = (fx32)(((fx64)det * var1) >> FX32_SHIFT);
    out->m[1][0] = -(fx32)(((fx64)det * det1) >> FX32_SHIFT);
    out->m[1][1] = (fx32)(((fx64)det * var2) >> FX32_SHIFT);
    out->m[1][2] = -(fx32)(((fx64)det * var3) >> FX32_SHIFT);
    out->m[2][0] = (fx32)(((fx64)det * det2) >> FX32_SHIFT);
    out->m[2][1] = -(fx32)(((fx64)det * (fx32)(((fx64)src->m[0][0] * src->m[2][1] - (fx64)src->m[2][0] * src->m[0][1]) >> FX32_SHIFT)) >> FX32_SHIFT);
    out->m[2][2] = (fx32)(((fx64)det * (fx32)(((fx64)src->m[0][0] * src->m[1][1] - (fx64)src->m[1][0] * src->m[0][1]) >> FX32_SHIFT)) >> FX32_SHIFT);

    if (out == &tmp) {
        MI_Copy36B(&tmp, dst);
    }
    return 0;
}
