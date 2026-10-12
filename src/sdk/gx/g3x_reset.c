#include "types.h"
#include "sdk/gx.h"

/* Per-scene reset of the geometry engine: waits until it is idle, clears the
 * error flags, resets the matrix stacks and sets default polygon attributes
 * (alpha 31, front faces rendered), texture parameters and palette base. */
void G3X_Reset(void) {
  while (*REG_GXSTAT & GXSTAT_GEOMETRY_BUSY) {
  }
  *REG_GXSTAT |= GXSTAT_MTX_STACK_ERROR_ACK;
  *REG_DISP3DCNT |= DISP3DCNT_UNDERFLOW_ACK;
  *REG_DISP3DCNT |= DISP3DCNT_OVERFLOW_ACK;
  G3X_ResetMtxStack();
  *REG_POLYGON_ATTR = 0x001f0080;
  *REG_TEXIMAGE_PARAM = 0;
  *REG_PLTT_BASE = 0;
}
