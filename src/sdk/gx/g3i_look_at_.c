#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"

/* Builds a camera matrix looking from camPos at target. With load set, it
 * also loads the matrix into the geometry engine's position/vector matrix;
 * with mtx not NULL, it also writes it there. */
void G3i_LookAt_(const VecFx32* camPos, const VecFx32* camUp,
                 const VecFx32* target, BOOL load, MtxFx43* mtx) {
  VecFx32 look;
  VecFx32 right;
  VecFx32 up;
  fx32 transX;
  fx32 transY;
  fx32 transZ;
  volatile s32* loadReg;

  look.x = camPos->x - target->x;
  look.y = camPos->y - target->y;
  look.z = camPos->z - target->z;
  VEC_Normalize(&look, &look);
  VEC_CrossProduct(camUp, &look, &right);
  VEC_Normalize(&right, &right);
  VEC_CrossProduct(&look, &right, &up);

  if (load) {
    *REG_MTX_MODE = GX_MTXMODE_POSITION_VECTOR;
    /* needed to match: the original keeps the register address in r4 */
    loadReg = IO_GEOM_SET_MATRIX_4X3;
    *loadReg = right.x;
    *loadReg = up.x;
    *loadReg = look.x;
    *loadReg = right.y;
    *loadReg = up.y;
    *loadReg = look.y;
    *loadReg = right.z;
    *loadReg = up.z;
    *loadReg = look.z;
  }

  transX = -VEC_DotProduct(camPos, &right);
  transY = -VEC_DotProduct(camPos, &up);
  transZ = -VEC_DotProduct(camPos, &look);
  if (load) {
    *loadReg = transX;
    *loadReg = transY;
    *loadReg = transZ;
  }

  if (mtx != NULL) {
    mtx->m[0][0] = right.x;
    mtx->m[0][1] = up.x;
    mtx->m[0][2] = look.x;
    mtx->m[1][0] = right.y;
    mtx->m[1][1] = up.y;
    mtx->m[1][2] = look.y;
    mtx->m[2][0] = right.z;
    mtx->m[2][1] = up.z;
    mtx->m[2][2] = look.z;
    mtx->m[3][0] = transX;
    mtx->m[3][1] = transY;
    mtx->m[3][2] = transZ;
  }
}
