#include "types.h"
#include "game/menu_button.h"
#include "game/link_menu_state.h"

extern const MenuButtonTemplate data_0215fefc;
extern const MenuButtonTemplate data_0215ff24;
extern const MenuButtonTemplate data_0215ff74;

/* Adds button 254 and the five row buttons, and selects the remembered one. */
void LinkMenu_AddAllButtons(void) {
  int i;

  MenuButton_Add(0, 0, &data_0215ff74, 254);
  for (i = 0; i < LINK_MENU_ROW_COUNT; i++) {
    if (i == LINK_MENU_ROW_COUNT - 1) {
      MenuButton_Add(0, i * 40, &data_0215ff24, i);
    } else {
      MenuButton_Add(0, i * 40, &data_0215fefc, i);
    }
  }
  MenuButton_Select(data_0215fed0->selected);
}
