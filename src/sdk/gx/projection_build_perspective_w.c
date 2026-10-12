#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"

/* Builds a perspective projection matrix for the view frustum whose near
 * plane spans left..right and bottom..top, with near and far planes and a
 * w scale. With load set, it loads the matrix as the geometry engine's
 * projection matrix; with mtx not NULL, it also writes it there. */
void Projection_BuildPerspectiveW(fx32 top, fx32 bottom, fx32 left, fx32 right,
                                  fx32 near, fx32 far, fx32 scaleW, BOOL load,
                                  MtxFx44* mtx) {
  long long invWidth;
  long long invHeight;
  long long invDepth;
  fx32 twoNear;
  fx32 x;
  fx32 y;
  fx32 offsetX;
  fx32 offsetY;
  fx32 z;
  fx32 w;
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
    mtx->m[2][3] = -0x1000;
    mtx->m[3][0] = 0;
    mtx->m[3][1] = 0;
    mtx->m[3][3] = 0;
  }

  twoNear = near * 2;
  invWidth = Divider_WaitRawQuotient64();
  Divider_StartFixedReciprocal(top - bottom);
  if (scaleW != 0x1000) {
    invWidth = invWidth * scaleW / 0x1000;
  }
  x = FX_Mul32x64c(twoNear, invWidth);
  if (load) {
    *loadReg = x;
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
  y = FX_Mul32x64c(twoNear, invHeight);
  if (load) {
    *loadReg = 0;
    *loadReg = y;
    *loadReg = 0;
    *loadReg = 0;
  }
  if (mtx != NULL) {
    mtx->m[1][1] = y;
  }

  offsetX = FX_Mul32x64c(right + left, invWidth);
  offsetY = FX_Mul32x64c(top + bottom, invHeight);
  if (load) {
    *loadReg = offsetX;
    *loadReg = offsetY;
  }
  if (mtx != NULL) {
    mtx->m[2][0] = offsetX;
    mtx->m[2][1] = offsetY;
  }

  invDepth = Divider_WaitRawQuotient64();
  if (scaleW != 0x1000) {
    invDepth = invDepth * scaleW / 0x1000;
  }
  z = FX_Mul32x64c(far + near, invDepth);
  w = FX_Mul32x64c(FX_Mul(twoNear, far), invDepth);
  if (load) {
    *loadReg = z;
    *loadReg = -scaleW;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = w;
    *loadReg = 0;
  }
  if (mtx != NULL) {
    mtx->m[2][2] = z;
    mtx->m[3][2] = w;
  }
}
