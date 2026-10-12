#include "types.h"
#include "game/screen_other_map.h"

void func_0201e6e8(int x, int y, int width, int height);
void Mode_PrintVariantValue(void* task);

/* Script step: on the second screen, sets the Y scroll of layers 0 and 1
 * to -192 (the next step slides them in from there), draws a 20 by 8
 * tile frame on layer 1 and prints the mode variant value. */
void PointsTally_InitSubScreen(void* task) {
  Unk_027c0280* savedGfx = data_027c0280;
  u8 frameBg;
  u8 textBg;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  frameBg = data_027c0280->layer1Bg;
  textBg = data_027c0280->layer0Bg;
  data_027c0280->bgScrollY[frameBg] = 0xff40;
  data_027c0280->bgScrollY[textBg] = data_027c0280->bgScrollY[frameBg];
  func_0201e6e8(6, 8, 20, 8);
  Mode_PrintVariantValue(task);
  Screen_SetCurrentGfx(savedGfx);
}
