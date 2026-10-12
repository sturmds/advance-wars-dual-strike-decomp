#include "types.h"
#include "game/menu_button.h"

/* Lets touch reach the active button with this id again. */
void MenuButton_EnableTouch(int id) {
  MenuButton* button = MenuButton_Find(id);

  if (button != NULL) {
    button->flags &= ~MENU_BUTTON_NO_TOUCH_CONFIRM;
  }
}
