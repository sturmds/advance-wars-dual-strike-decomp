#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Turns off the three windows and clears the blend targets (all four layers
 * and OBJ), the blend mode and the blend coefficients. */
void Ov006_ClearWindowsAndBlend(void) {
  GFX_DISPCNT(data_027c0280)->win0 = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
}
