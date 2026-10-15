#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

/* Puts layer 1 in front, then layer 3, then layers 0 and 2, and
 * alpha-blends layer 0 over layers 2 and 3, the sprites and the backdrop
 * at EVA 8, EVB 16. One screen config record (data_02169f48) points six of
 * its hooks to it. */
void Screen_SetLayer0Translucent(void) {
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 3;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 3;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 2;
  Screen_ResetBlend();
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 8;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
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
