#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

void Screen_SetLayerPriorities(void);

/* Resets the layer priorities and windows and alpha-blends layer 1 over
 * layers 2 and 3, the sprites and the backdrop at EVA 6, EVB 16. A screen
 * config record (data_02169f48) points to it. */
void Screen_SetLayer1Translucent(void) {
  Screen_SetLayerPriorities();
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 6;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->blendBottomBackdrop = 1;
  Screen_ResetWindows();
}
