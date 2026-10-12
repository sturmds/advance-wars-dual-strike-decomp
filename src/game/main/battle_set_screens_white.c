#include "types.h"
#include "game/screen_other_map.h"

void func_020b97e8(void);
void func_020f4768(void);

/* Sets the master brightness of `screen` to white, then restores the current screen. */
#define SCREEN_SET_WHITE(screen)             \
  {                                          \
    Unk_027c0280* saved0280 = data_027c0280; \
    Unk_02080440* saved027c;                 \
    Screen_SetCurrentGfx(screen);            \
    saved027c = data_027c027c;               \
    Screen_SetCurrent(screen);               \
    data_027c0280->masterBrightness = 16;    \
    Screen_SetCurrent(saved027c);            \
    Screen_SetCurrentGfx(saved0280);         \
  }

void Battle_SetScreensWhite(void) {
  func_020b97e8();
  func_020f4768();
  SCREEN_SET_WHITE(data_027c0288.unk_00);
  SCREEN_SET_WHITE(data_027c0288.unk_04);
}
