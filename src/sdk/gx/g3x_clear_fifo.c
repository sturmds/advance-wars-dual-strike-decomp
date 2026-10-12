#include "types.h"
#include "sdk/gx.h"

/* Pushes NOPs through the geometry FIFO, then waits until the geometry
 * engine is idle. */
void G3X_ClearFifo(void) {
  GXi_NopClearFifo128_(REG_GXFIFO);
  while (*REG_GXSTAT & GXSTAT_GEOMETRY_BUSY) {
  }
}
