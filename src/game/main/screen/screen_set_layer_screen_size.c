#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void BgControl_SetScreenSize(BgControl* bg, int screenSize);

/* Sets the screen size (BGxCNT bits 14-15) of layer 0-3 and its BG control register. */
void Screen_SetLayerScreenSize(int layer, int screenSize) {
  GFX_LAYER(data_027c0280, layer)->screenSize = screenSize;
  BgControl_SetScreenSize(
      &data_027c0280->bgCnt[GFX_LAYER(data_027c0280, layer)->bg], screenSize);
}
