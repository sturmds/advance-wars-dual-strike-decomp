#include "types.h"
#include "sdk/gx.h"

/* Writes BLDCNT, BLDALPHA and BLDY at reg (a blend control register block).
 * evy > 0 brightens, evy < 0 darkens. */
void G2x_SetBlendBrightnessExt(volatile u16* reg, int top, int bottom, int eva,
                               int evb, int evy) {
  reg[1] = eva | (evb << 8);
  if (evy < 0) {
    reg[0] = top | BLDCNT_EFFECT_DARKEN | (bottom << 8);
    reg[2] = -evy;
  } else {
    reg[0] = top | BLDCNT_EFFECT_BRIGHTEN | (bottom << 8);
    reg[2] = evy;
  }
}
