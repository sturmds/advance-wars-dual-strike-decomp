#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

/* Script step: on the sub screen, shows BG2, BG3 and the sprites, and sets
 * an alpha blend of layer 2 over layer 3 with EVA 0 and EVB 16, so layer 2
 * starts out invisible. SubScreenLayer2_SlideInUpdate runs next. */
void SubScreenLayer2_SlideInStart(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 2)->bg] = 1;
  data_027c0280->blendTopBg[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->blendBottomBg[GFX_LAYER(data_027c0280, 3)->bg] = 1;
  data_027c0280->blendBottomObj = 0;
  GFX_DISPCNT(data_027c0280)->bg0 = 0;
  GFX_DISPCNT(data_027c0280)->bg1 = 0;
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 1;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
