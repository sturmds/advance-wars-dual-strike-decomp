#include "types.h"
#include "game/menu_button.h"

int MenuButton_TestFlags(MenuButton* entry, int mask);

/* Deactivates every button without MENU_BUTTON_SELECT_ON_TOUCH. */
void MenuButtons_RemoveUnselectable(void) {
  int i;
  MenuButton* entry;

  entry = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, entry++) {
    if (MenuButton_TestFlags(entry, MENU_BUTTON_SELECT_ON_TOUCH) !=
        MENU_BUTTON_SELECT_ON_TOUCH) {
      entry->active = 0;
    }
  }
}
