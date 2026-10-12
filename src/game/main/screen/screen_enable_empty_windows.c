#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0216e2b0.h"

extern Unk_027c0280* data_027c0280;

void Screen_EnableEmptyWindows(void) {
  GFX_DISPCNT(data_027c0280)->win0 = 1;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->win0In.obj = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 1;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->win1In.obj = 0;
  data_027c0280->win1Left = 255;
  data_027c0280->win1Top = 0;
  data_027c0280->win1Right = 1;
  data_027c0280->win1Bottom = 0;
  data_0216e2b0.unk_02 = 0;
  data_0216e2b0.unk_03 = 0;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 0)->bg] = 1;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 1)->bg] = 1;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 2)->bg] = 1;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 3)->bg] = 1;
  data_027c0280->winOut.obj = 1;
  data_027c0280->winOut.effect = 1;
}
