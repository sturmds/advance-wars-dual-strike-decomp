#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Script step before SubScreenLayers01_SlideInUpdate: on the sub screen, starts
 * layers 0 and 1 192 pixels to the right (BG x scroll 192) and shows all four
 * layers and the sprites. */
void SubScreenLayers01_SlideInStart(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int layer1;
  int layer0;

  FRAME(task) = 0;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  layer1 = data_027c0280->layer1Bg;
  layer0 = data_027c0280->layer0Bg;
  data_027c0280->bgScrollX[layer1] = 192;
  data_027c0280->bgScrollX[layer0] = data_027c0280->bgScrollX[layer1];
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 1;
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 1;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
