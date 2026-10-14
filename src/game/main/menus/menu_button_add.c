#include "types.h"
#include "game/menu_button.h"

/* Makes a button from template at (x, y) in the first free slot of
 * gMenuButtons, leaving out the reserved last one. Returns NULL when all
 * slots are in use. */
MenuButton* MenuButton_Add(int x, int y, const MenuButtonTemplate* template,
                           int id) {
  int i;
  MenuButton* button = gMenuButtons;

  for (i = 0; i < MENU_BUTTON_COUNT - 1; i++, button++) {
    if (!button->active) {
      return MenuButton_Init(x, y, &gMenuButtons[i], template, id);
    }
  }
  return NULL;
}
