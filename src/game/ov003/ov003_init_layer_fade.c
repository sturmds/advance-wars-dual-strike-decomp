#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_0235dd38.h"

extern Unk_027c0280* data_027c0280;

/* Sets up a fade on the four BG layers: blend mode 3 (brightness decrease)
 * with the layers as top targets, nothing else, and all coefficients 0. */
void Ov003_InitLayerFade(Unk_ov003_0235dd38* obj) {
  obj->unk_4c = 0;
  data_027c0280->blendMode = 3;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
}
