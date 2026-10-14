#include "types.h"
#include "game/menu_button.h"

int MenuButton_TestFlags(MenuButton* button, int mask);

/* Removes every selectable button at once, without the closing animation. */
void MenuButtons_RemoveSelectable(void) {
  int i;
  MenuButton* button = gMenuButtons;

  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (MenuButton_TestFlags(button, MENU_BUTTON_SELECT_ON_TOUCH)) {
      button->active = 0;
    }
  }
}
