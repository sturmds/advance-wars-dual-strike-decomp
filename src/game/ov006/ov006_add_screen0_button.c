#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

void func_ov006_02355584(void);

static MenuButtonTemplate data_ov006_0236b634 = {
#include "data/ov006/data_ov006_0236b634.inc.c"
};

void Ov006_AddScreen0Button(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(0, 0, &data_ov006_0236b634, 241);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
