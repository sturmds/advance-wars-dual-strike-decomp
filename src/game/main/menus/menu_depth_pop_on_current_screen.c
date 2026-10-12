#include "types.h"
#include "game/screen_other_map.h"

void MenuDepth_Pop(void);

/* Runs MenuDepth_Pop with the current screen's state and gfx state loaded
 * from data_027c0288, then restores the previous ones. */
void MenuDepth_PopOnCurrentScreen(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  savedState = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  MenuDepth_Pop();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
