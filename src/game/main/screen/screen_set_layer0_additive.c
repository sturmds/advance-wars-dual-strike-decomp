#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

/* Puts layer 1 in front, then layers 0, 2 and 3, and adds layer 0 onto
 * layers 2 and 3 and the backdrop (EVA 16, EVB 16; sprites are not a
 * target). A screen config record (data_02169f48) points to it. */
void Screen_SetLayer0Additive(void) {
  Screen_SetLayer1Front();
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 16;
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
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendBottomBackdrop = 1;
  Screen_ResetWindows();
}
