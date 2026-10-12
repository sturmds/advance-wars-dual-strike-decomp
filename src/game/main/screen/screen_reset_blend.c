#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Screen_ResetBlend(void) {
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
}
