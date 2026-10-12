#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

extern const MenuButtonTemplate data_ov006_0236b65c;

void func_ov006_02355514(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(0, 0, &data_ov006_0236b65c, 240);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
