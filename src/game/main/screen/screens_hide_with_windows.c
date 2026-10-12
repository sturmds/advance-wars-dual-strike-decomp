#include "types.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void func_02006610(void);

/* Hides both screens: func_02006610 turns on empty windows and marks the
 * screen hidden. */
void Screens_HideWithWindows(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  func_02006610();
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  func_02006610();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
