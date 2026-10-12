#include "types.h"
#include "game/screen_other_map.h"

void Screen_ResetBlendAllTop(void);
void Screen_ApplyAll(void);

/* Turns the current screen fully black: darken blend at level 16 / 16. */
void Screen_FillBlack(void) {
  Screen_ResetBlendAllTop();
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 16;
  Screen_ApplyAll();
}
