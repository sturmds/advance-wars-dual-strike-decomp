#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbb0.h"

void Ov005_RemoveShopButtons(Unk_ov005_0236bbb0* work) {
  ShopButton* button;
  int i;

  for (i = 0; i < SHOP_PAGE_ROWS; i++) {
    button = &work->buttons[i];

    if (button->shown != 0) {
      Unk_027c0280* saved0280;
      Unk_02080440* saved027c;

      button->shown = 0;
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      MenuButton_Remove(button->buttonId);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  }
}
