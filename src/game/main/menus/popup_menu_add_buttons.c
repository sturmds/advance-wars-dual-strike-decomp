#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"

extern MenuButtonTemplate data_02158780;
extern MenuButtonTemplate data_02158f54;

/* Selects the menu cursor's button and adds one button per item, two tile rows
 * apart, each as wide as the menu, then a reserved button. */
void PopupMenu_AddButtons(PopupMenuTask* menu) {
  int i;
  MenuButton* button;

  MenuButton_Select(menu->selected);
  for (i = 0; i < menu->count; i++) {
    button = MenuButton_Add((menu->x + 1) * 8, (menu->y + 1 + i * 2) * 8,
                            &data_02158780, i);
    button->width = menu->width * 8;
    button->height = 16;
    button->owner = menu;
  }
  MenuButton_AddReserved(0, 0, &data_02158f54);
}
