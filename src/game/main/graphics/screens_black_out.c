#include "types.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);

/* Blacks out both screens: darken blend at full strength, master brightness 0. */
void Screens_BlackOut(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEvy = 16;
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEvy = 16;
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
