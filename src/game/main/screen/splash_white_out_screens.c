#include "types.h"
#include "game/screen_other_map.h"
#include "game/splash_task.h"

void Screen_SetCurrentBoth(void* state);

/* Brighten blend at full strength on every BG layer, the sprites and the
 * backdrop. */
#define WHITE_OUT()                                       \
  data_027c0280->blendMode = BLEND_MODE_BRIGHTEN;         \
  data_027c0280->blendEva = 0;                            \
  data_027c0280->blendEvb = 0;                            \
  data_027c0280->blendEvy = 16;                           \
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1; \
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1; \
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1; \
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1; \
  data_027c0280->blendTopObj = 1;                         \
  data_027c0280->blendTopBackdrop = 1

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Task step of script data_02169ccc: resets the master brightness of both
 * screens to 0 and turns both screens white with a full brighten blend. */
void Splash_WhiteOutScreens(SplashTask* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = 0;
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_00);
  WHITE_OUT();
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_04);
  WHITE_OUT();
  SWITCH_BACK();
}
