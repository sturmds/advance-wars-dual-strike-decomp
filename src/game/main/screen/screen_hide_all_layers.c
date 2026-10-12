#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Screen_HideAllLayers(void) {
  GFX_DISPCNT(data_027c0280)->bg0 = 0;
  GFX_DISPCNT(data_027c0280)->bg1 = 0;
  GFX_DISPCNT(data_027c0280)->bg2 = 0;
  GFX_DISPCNT(data_027c0280)->bg3 = 0;
  GFX_DISPCNT(data_027c0280)->obj = 0;
}
