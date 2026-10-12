#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Script step: on both screens, turns the windows off and darkens all four
 * layers and the sprites fully (16 / 16). ScreenDarkenIn_Update, next in
 * the script, then lowers it to 0. */
void ScreenDarkenIn_Start(Task* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 16;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 0)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 1)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 2)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 3)->bg] = 1;
  data_027c0280->blendTopObj = 1;
  GFX_DISPCNT(data_027c0280)->win0 = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;

  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 16;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 0)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 1)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 2)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 3)->bg] = 1;
  data_027c0280->blendTopObj = 1;
  GFX_DISPCNT(data_027c0280)->win0 = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;

  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  FRAME(task) = 0;
}
