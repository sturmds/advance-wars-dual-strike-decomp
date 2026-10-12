#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"

/* Builds an orthographic projection matrix for the box left..right,
 * bottom..top, near..far, with scaleW as the w scale. With load set, it
 * loads the matrix as the geometry engine's projection matrix; with mtx not
 * NULL, it also writes it there. */
void G3i_OrthoW_(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 near,
                 fx32 far, fx32 scaleW, BOOL load, MtxFx44* mtx) {
  long long invWidth;
  long long invHeight;
  long long invDepth;
  fx32 x;
  fx32 y;
  fx32 z;
  volatile s32* loadReg;

  FX_InvAsync(right - left);
  if (load) {
    *REG_MTX_MODE = GX_MTXMODE_PROJECTION;
    /* needed to match: the original keeps the register address in r4 */
    loadReg = IO_GEOM_SET_MATRIX_4X4;
  }
  if (mtx != NULL) {
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[0][3] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][2] = 0;
    mtx->m[1][3] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][3] = 0;
    mtx->m[3][3] = scaleW;
  }

  invWidth = Divider_WaitRawQuotient64();
  Divider_StartFixedReciprocal(top - bottom);
  if (scaleW != 0x1000) {
    invWidth = invWidth * scaleW / 0x1000;
  }
  x = FX_Mul32x64c(0x2000, invWidth);
  if (load) {
    *loadReg = x;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
  }
  if (mtx != NULL) {
    mtx->m[0][0] = x;
  }

  invHeight = Divider_WaitRawQuotient64();
  Divider_StartFixedReciprocal(near - far);
  if (scaleW != 0x1000) {
    invHeight = invHeight * scaleW / 0x1000;
  }
  y = FX_Mul32x64c(0x2000, invHeight);
  if (load) {
    *loadReg = y;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
  }
  if (mtx != NULL) {
    mtx->m[1][1] = y;
  }

  invDepth = Divider_WaitRawQuotient64();
  if (scaleW != 0x1000) {
    invDepth = invDepth * scaleW / 0x1000;
  }
  z = FX_Mul32x64c(0x2000, invDepth);
  if (load) {
    *loadReg = z;
    *loadReg = 0;
  }
  if (mtx != NULL) {
    mtx->m[2][2] = z;
  }

  x = FX_Mul32x64c(-(right + left), invWidth);
  y = FX_Mul32x64c(-(top + bottom), invHeight);
  z = FX_Mul32x64c(far + near, invDepth);
  if (load) {
    *loadReg = x;
    *loadReg = y;
    *loadReg = z;
    *loadReg = scaleW;
  }
  if (mtx != NULL) {
    mtx->m[3][0] = x;
    mtx->m[3][1] = y;
    mtx->m[3][2] = z;
  }
}
