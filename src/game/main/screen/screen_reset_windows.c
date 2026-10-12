#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Screen_ResetWindows(void) {
  GFX_DISPCNT(data_027c0280)->win0 = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  data_027c0280->win0Left = 0;
  data_027c0280->win0Top = 0;
  data_027c0280->win0Right = 0;
  data_027c0280->win0Bottom = 0;
  data_027c0280->win1Left = 0;
  data_027c0280->win1Top = 0;
  data_027c0280->win1Right = 0;
  data_027c0280->win1Bottom = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->win0In.bg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->win0In.obj = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->win1In.bg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->win1In.obj = 0;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->winOut.bg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->winOut.obj = 0;
  data_027c0280->objWinIn.bg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->objWinIn.bg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->objWinIn.bg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->objWinIn.bg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->objWinIn.obj = 0;
  data_027c0280->win0In.effect = 0;
  data_027c0280->win1In.effect = 0;
  data_027c0280->winOut.effect = 0;
  data_027c0280->objWinIn.effect = 0;
}
