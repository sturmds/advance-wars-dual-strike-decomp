#include "types.h"
#include "sdk/gx.h"

/* Same as G3X_ResetMtxStack, and also loads identity into the projection
 * matrix. */
void G3X_InitMtxStack(void) {
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
  *REG_MTX_IDENTITY = 0;
  *REG_MTX_MODE = GX_MTXMODE_POSITION_VECTOR;
  *REG_MTX_POP = pvLevel;
  *REG_MTX_IDENTITY = 0;
}
