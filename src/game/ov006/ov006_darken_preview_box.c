#include "types.h"
#include "game/Unk_02088d04.h"

void Ov006_SetUpDarkenWindow(int left, int top, int right, int bottom);

/* Called after a design map's preview is drawn (func_ov006_023582f0).
 * Ov006_SetUpDarkenWindow turns on window 0 with colour effects inside it only;
 * this moves window 0 to the box (127, 39)-(249, 121) and darkens layers 1-3
 * and sprites by 9/16 there. Layer 0, which holds the menu text, stays bright. */
void Ov006_DarkenPreviewBox(void) {
  Ov006_SetUpDarkenWindow(0, 256, 192, 192);
  GFX_DISPCNT(data_027c0280)->win0 = 1;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 9;
  data_027c0280->win0Left = 127;
  data_027c0280->win0Top = 39;
  data_027c0280->win0Right = 249;
  data_027c0280->win0Bottom = 121;
}
