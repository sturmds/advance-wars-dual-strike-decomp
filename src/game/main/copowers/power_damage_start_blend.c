#include "types.h"
#include "game/power_damage.h"
#include "game/screen_other_map.h"

/* Script step of gPowerDamageTaskDesc: starts an alpha blend of layer 0
 * over layers 1 to 3 and the backdrop, with EVA 0 and EVB 16, so layer 0
 * begins fully transparent. */
void PowerDamage_StartBlend(PowerDamageTask* task) {
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->blendBottomBackdrop = 1;
  task->blendFrame = 0;
}
