#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void MenuDepth_Pop(void);

/* Pops the menu depth on the screen of data_027c0288.unk_00, then on the one
 * of unk_04. The current screen and gfx state are restored after each pop. */
void MenuDepth_PopBothScreens(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuDepth_Pop();
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  MenuDepth_Pop();
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
