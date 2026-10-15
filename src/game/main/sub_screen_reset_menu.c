#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/menu_button.h"

void func_02082a78(void);
void func_02088514(void);

void SubScreen_ResetMenu(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_02082a78();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  MenuButtons_RemoveAll();
  func_02088514();
}
