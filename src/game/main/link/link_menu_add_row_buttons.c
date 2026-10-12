#include "types.h"
#include "game/menu_button.h"
#include "game/link_menu_state.h"

extern const MenuButtonTemplate data_0215fefc;
extern const MenuButtonTemplate data_0215ff24;

int func_0207a260(void);

/* Adds the five row buttons (ids 0-4, 40 pixels apart; button 4 has its own
 * template), closes button 253 when func_0207a260 returns 0, and selects the
 * remembered button. */
void LinkMenu_AddRowButtons(void) {
  int i;

  for (i = 0; i < 5; i++) {
    if (i == 4) {
      MenuButton_Add(0, i * 40, &data_0215ff24, i);
    } else {
      MenuButton_Add(0, i * 40, &data_0215fefc, i);
    }
  }
  if (func_0207a260() == 0) {
    MenuButton_Close(253);
  }
  MenuButton_Select(data_0215fed0->selected);
}
