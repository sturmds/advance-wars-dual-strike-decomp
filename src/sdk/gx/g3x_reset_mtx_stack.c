#include "types.h"
#include "sdk/gx.h"

/* Clears the stack error flag, pops the projection and position/vector
 * stacks back to level 0, and loads identity into the texture and
 * position/vector matrices. */
void G3X_ResetMtxStack(void) {
  int pvLevel;
  int pjLevel;

  *REG_GXSTAT |= GXSTAT_MTX_STACK_ERROR_ACK;
  while (G3X_GetMtxStackLevelPV(&pvLevel)) {
  }
  while (G3X_GetMtxStackLevelPJ(&pjLevel)) {
  }

  *REG_MTX_MODE = GX_MTXMODE_TEXTURE;
  *REG_MTX_IDENTITY = 0;
  *REG_MTX_MODE = GX_MTXMODE_PROJECTION;
  if (pjLevel != 0) {
    *REG_MTX_POP = pjLevel;
  }
  *REG_MTX_MODE = GX_MTXMODE_POSITION_VECTOR;
  *REG_MTX_POP = pvLevel;
  *REG_MTX_IDENTITY = 0;
}
