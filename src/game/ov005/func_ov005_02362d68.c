#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

extern const MenuButtonTemplate data_ov005_0236ab84;

void func_ov005_02362d68(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(0, 0, &data_ov005_0236ab84, 241);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
