#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"

/* Builds a perspective projection matrix from the sine and cosine of the
 * vertical field of view, the aspect ratio, the near and far planes and a
 * w scale. With load set, it loads the matrix as the geometry engine's
 * projection matrix; with mtx not NULL, it also writes it there. */
void G3i_PerspectiveW_(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 near,
                       fx32 far, fx32 scaleW, BOOL load, MtxFx44* mtx) {
  fx32 fovyCot;
  fx32 x;
  fx32 z;
  fx32 w;
  long long invDepth;
  volatile s32* loadReg;

  fovyCot = FX_Div(fovyCos, fovySin);
  if (scaleW != 0x1000) {
    fovyCot = fovyCot * scaleW / 0x1000;
  }
  Divider_StartFixedQuotient(fovyCot, aspect);
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
    mtx->m[2][3] = -scaleW;
    mtx->m[3][0] = 0;
    mtx->m[3][1] = 0;
    mtx->m[3][3] = 0;
  }

  x = FX_GetDivResult();
  Divider_StartFixedReciprocal(near - far);
  if (load) {
    *loadReg = x;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = fovyCot;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
    *loadReg = 0;
  }
  if (mtx != NULL) {
    mtx->m[0][0] = x;
    mtx->m[1][1] = fovyCot;
  }

  invDepth = Divider_WaitRawQuotient64();
  if (scaleW != 0x1000) {
    invDepth = invDepth * scaleW / 0x1000;
  }
  z = FX_Mul32x64c(far + near, invDepth);
  w = FX_Mul32x64c(FX_Mul(near * 2, far), invDepth);
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
