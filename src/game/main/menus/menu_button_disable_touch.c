#include "types.h"
#include "game/menu_button.h"

/* Sets MENU_BUTTON_NO_TOUCH_CONFIRM on the active button with this id; MenuButton_EnableTouch clears it. */
void MenuButton_DisableTouch(int id) {
  MenuButton* button;

  button = MenuButton_Find(id);
  if (button != NULL) {
    button->flags |= MENU_BUTTON_NO_TOUCH_CONFIRM;
  }
}
