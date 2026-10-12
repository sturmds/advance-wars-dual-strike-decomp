#include "types.h"
#include "sdk/gx.h"

/* Writes BLDCNT (alpha blend) and BLDALPHA as one word at reg. */
void G2x_SetBlendAlpha(volatile u32* reg, int top, int bottom, int eva,
                       int evb) {
  *reg =
      (top | BLDCNT_EFFECT_ALPHA | (bottom << 8)) | ((eva | (evb << 8)) << 16);
}
