#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void func_020323fc(void);
void ResultPoint_Close(void);

/* Brighten blend at full strength on every BG layer and the sprites. */
#define WHITE_OUT()                                       \
  data_027c0280->blendMode = BLEND_MODE_BRIGHTEN;         \
  data_027c0280->blendEva = 0;                            \
  data_027c0280->blendEvb = 0;                            \
  data_027c0280->blendEvy = 16;                           \
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1; \
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1; \
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1; \
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1; \
  data_027c0280->blendTopObj = 1

/* Destroys the data_02159ca8 task and the menu buttons, runs
 * ResultPoint_Close, and turns both screens white. */
void ResultScreen_CloseToWhite(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  func_020323fc();
  MenuButtons_RemoveAll();
  ResultPoint_Close();

  savedGfx = data_027c0280;
  savedState = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  WHITE_OUT();
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  WHITE_OUT();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
