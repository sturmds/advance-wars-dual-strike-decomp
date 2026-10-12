#include "types.h"
#include "game/screen_other_map.h"

/* Sets window 0 to (left, top)-(right, bottom) and turns it on, with the
 * colour effect (darken by 16) outside it. */
void Ov006_SetUpDarkenWindow(int left, int top, int right, int bottom);

/* On screen data_027c0288.unk_04: turns all windows off, leaves alpha blend
 * with no first target (so nothing blends), and sets window 0 to
 * (128, 24)-(255, 183). The call to Ov006_SetUpDarkenWindow is undone right
 * after, except for its window plane settings. */
void Ov006_ResetScreen1Effects(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Ov006_SetUpDarkenWindow(0, 256, 192, 192);
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 14;
  data_027c0280->blendEvy = 0;
  data_027c0280->win0Left = 128;
  data_027c0280->win0Top = 24;
  data_027c0280->win0Right = 255;
  data_027c0280->win0Bottom = 183;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
