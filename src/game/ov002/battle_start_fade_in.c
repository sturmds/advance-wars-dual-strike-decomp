#include "types.h"
#include "game/screen_other_map.h"

/* Starts the fade from black: like Battle_StartFadeOut but BLDY starts at 16;
 * Battle_FadeInStep lowers it. */
void Battle_StartFadeIn(void) {
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 16;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 1;
}
