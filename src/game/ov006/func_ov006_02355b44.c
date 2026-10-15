#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_02355b44(void) {
  int buttonId;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (buttonId = 64; buttonId <= data_ov006_0236d640->unk_50; buttonId++) {
    MenuButton_Remove(buttonId);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
