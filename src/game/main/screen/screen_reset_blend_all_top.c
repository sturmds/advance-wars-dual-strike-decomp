#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Turns colour effects off on the current screen and sets the targets the
 * blend fades use: all four layers, the sprites and the backdrop as first
 * target, nothing as second target, all levels at 0. */
void Screen_ResetBlendAllTop(void) {
  data_027c0280->blendMode = BLEND_MODE_NONE;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 0)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 1)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 2)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 3)->bg] = 1;
  data_027c0280->blendTopObj = 1;
  data_027c0280->blendTopBackdrop = 1;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
}
