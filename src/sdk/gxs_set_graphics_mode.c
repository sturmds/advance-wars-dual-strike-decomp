#include "types.h"
#include "sdk/gx.h"

/* Sets the sub engine's BG mode (DISPCNT bits 0-2). */
void GXS_SetGraphicsMode(int bgMode) {
  *REG_DB_DISPCNT = (*REG_DB_DISPCNT & ~7) | bgMode;
}
