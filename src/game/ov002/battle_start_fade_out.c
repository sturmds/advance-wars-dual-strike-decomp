#include "types.h"
#include "game/screen_other_map.h"

/* Starts the fade to black: darken mode with BLDY 0 on the four layers and
 * the sprites. Battle_FadeOutStep then raises BLDY. */
void Battle_StartFadeOut(void) {
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 1;
}
