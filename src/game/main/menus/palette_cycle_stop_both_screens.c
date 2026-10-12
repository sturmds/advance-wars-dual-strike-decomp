#include "types.h"
#include "game/screen_other_map.h"

void PaletteCycle_Stop(void);

/* Runs PaletteCycle_Stop (which destroys the task of PaletteCycle_Start's script)
 * for the current screen and for the second screen. */
void PaletteCycle_StopBothScreens(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  PaletteCycle_Stop();
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  PaletteCycle_Stop();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
