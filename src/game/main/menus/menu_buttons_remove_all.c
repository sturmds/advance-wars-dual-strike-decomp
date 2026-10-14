#include "types.h"
#include "game/menu_button.h"

/* Removes every button at once, without the closing animation. */
void MenuButtons_RemoveAll(void) {
  int i;
  MenuButton* button = gMenuButtons;

  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    button->active = 0;
  }
}
