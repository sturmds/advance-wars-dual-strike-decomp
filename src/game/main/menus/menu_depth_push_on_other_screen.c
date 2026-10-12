#include "types.h"
#include "game/screen_other_map.h"

void MenuDepth_Push(void);

/* Runs MenuDepth_Push with the other screen's state loaded, then restores
 * the current screen's state. */
void MenuDepth_PushOnOtherScreen(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  savedState = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  MenuDepth_Push();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
