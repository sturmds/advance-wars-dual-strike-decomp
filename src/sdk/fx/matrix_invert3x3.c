#include "sdk/fx.h"
#include "sdk/mi.h"

/* Matrix_Invert3x3: writes the inverse of src to dst (they may be the
 * same matrix). Returns -1 and leaves dst alone when src has no inverse.
 * det0 to det2 are the cofactors of the first row; the reciprocal of the
 * determinant runs on the hardware divider while the other cofactors are
 * worked out. */
int Matrix_Invert3x3(const MtxFx33* src, MtxFx33* dst) {
  MtxFx33 tmp;
  MtxFx33* out;
  fx32 det;
  fx32 det0, det1, det2;
  fx32 var0, var1, var2, var3;

  out = (src == dst) ? &tmp : dst;

  det0 = (fx32)(((fx64)src->m[1][1] * src->m[2][2] -
                 (fx64)src->m[1][2] * src->m[2][1] + FX32_HALF) >>
                FX32_SHIFT);
  det1 = (fx32)(((fx64)src->m[1][0] * src->m[2][2] -
                 (fx64)src->m[1][2] * src->m[2][0] + FX32_HALF) >>
                FX32_SHIFT);
  det2 = (fx32)(((fx64)src->m[1][0] * src->m[2][1] -
                 (fx64)src->m[1][1] * src->m[2][0] + FX32_HALF) >>
                FX32_SHIFT);
  det = (fx32)(((fx64)src->m[0][0] * det0 - (fx64)src->m[0][1] * det1 +
                (fx64)src->m[0][2] * det2 + FX32_HALF) >>
               FX32_SHIFT);
  if (det == 0) {
    return -1;
  }
  FX_InvAsync(det);

  var0 = (fx32)(((fx64)src->m[0][1] * src->m[2][2] -
                 (fx64)src->m[2][1] * src->m[0][2]) >>
                FX32_SHIFT);
  var1 = (fx32)(((fx64)src->m[0][1] * src->m[1][2] -
                 (fx64)src->m[1][1] * src->m[0][2]) >>
                FX32_SHIFT);
  var2 = (fx32)(((fx64)src->m[0][0] * src->m[2][2] -
                 (fx64)src->m[2][0] * src->m[0][2]) >>
                FX32_SHIFT);
  var3 = (fx32)(((fx64)src->m[0][0] * src->m[1][2] -
                 (fx64)src->m[1][0] * src->m[0][2]) >>
                FX32_SHIFT);

  det = FX_GetDivResult();

  out->m[0][0] = (fx32)(((fx64)det * det0) >> FX32_SHIFT);
  out->m[0][1] = -(fx32)(((fx64)det * var0) >> FX32_SHIFT);
  out->m[0][2] = (fx32)(((fx64)det * var1) >> FX32_SHIFT);
  out->m[1][0] = -(fx32)(((fx64)det * det1) >> FX32_SHIFT);
  out->m[1][1] = (fx32)(((fx64)det * var2) >> FX32_SHIFT);
  out->m[1][2] = -(fx32)(((fx64)det * var3) >> FX32_SHIFT);
  out->m[2][0] = (fx32)(((fx64)det * det2) >> FX32_SHIFT);
  out->m[2][1] =
      -(fx32)(((fx64)det * (fx32)(((fx64)src->m[0][0] * src->m[2][1] -
                                   (fx64)src->m[2][0] * src->m[0][1]) >>
                                  FX32_SHIFT)) >>
              FX32_SHIFT);
  out->m[2][2] =
      (fx32)(((fx64)det * (fx32)(((fx64)src->m[0][0] * src->m[1][1] -
                                  (fx64)src->m[1][0] * src->m[0][1]) >>
                                 FX32_SHIFT)) >>
             FX32_SHIFT);

  if (out == &tmp) {
    MI_Copy36B(&tmp, dst);
  }
  return 0;
}
