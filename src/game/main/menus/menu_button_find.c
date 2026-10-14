#include "types.h"
#include "game/menu_button.h"

MenuButton* MenuButton_Find(int id) {
  int i;
  MenuButton* button;

  button = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (button->active && id == button->id) {
      /* needed to match: the original indexes the array again */
      return &gMenuButtons[i];
    }
  }
  return NULL;
}
