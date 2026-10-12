#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

void func_ov006_02355444(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Remove(240);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
