#include "types.h"
#include "game/Unk_02088d04.h"

void Screen_ResetWindows(void);

extern Unk_027c0280* data_027c0280;

#define GFX data_027c0280

/* Sets window 0 to the rectangle (left, top) - (right, bottom) and sets up
 * darkening of layer 3 and sprites inside it, starting at strength 0
 * (blendEvy). All layers and sprites stay shown inside and outside the
 * window. */
void Ov006_SetUpDarkenWindow(int left, int top, int right, int bottom) {
  GFX->blendMode = BLEND_MODE_DARKEN;
  GFX->blendEva = 16;
  GFX->blendEvb = 0;
  GFX->blendEvy = 0;
  GFX->blendTopBg[GFX->layer0Bg] = 0;
  GFX->blendTopBg[GFX->layer1Bg] = 0;
  GFX->blendTopBg[GFX->layer2Bg] = 0;
  GFX->blendTopBg[GFX->layer3Bg] = 1;
  GFX->blendTopObj = 1;
  GFX->blendBottomBg[GFX->layer0Bg] = 0;
  GFX->blendBottomBg[GFX->layer1Bg] = 1;
  GFX->blendBottomBg[GFX->layer2Bg] = 1;
  GFX->blendBottomBg[GFX->layer3Bg] = 1;
  GFX->blendBottomObj = 1;
  Screen_ResetWindows();
  GFX_DISPCNT(GFX)->win0 = 1;
  GFX->win0Left = left;
  GFX->win0Top = top;
  GFX->win0Right = right;
  GFX->win0Bottom = bottom;
  GFX->win0In.bg[GFX->layer0Bg] = 1;
  GFX->win0In.bg[GFX->layer1Bg] = 1;
  GFX->win0In.bg[GFX->layer2Bg] = 1;
  GFX->win0In.bg[GFX->layer3Bg] = 1;
  GFX->win0In.obj = 1;
  GFX->win0In.effect = 1;
  GFX->winOut.bg[GFX->layer0Bg] = 1;
  GFX->winOut.bg[GFX->layer1Bg] = 1;
  GFX->winOut.bg[GFX->layer2Bg] = 1;
  GFX->winOut.bg[GFX->layer3Bg] = 1;
  GFX->winOut.obj = 1;
  GFX->winOut.effect = 0;
}
