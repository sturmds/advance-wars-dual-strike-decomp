#include "types.h"
#include "sdk/gx.h"

/* Switches the main display back on with the mode GX_SetGraphicsMode last asked for (mode 1 if none). */
void GX_DispOn(void) {
  u16 dispMode;

  gGxDispOn = 1;
  dispMode = gGxDispMode;
  if (dispMode != 0) {
    *REG_DISPCNT = (*REG_DISPCNT & ~0x30000) | (dispMode << 16);
  } else {
    *REG_DISPCNT |= 0x10000;
  }
}
