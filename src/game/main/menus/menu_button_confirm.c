#include "types.h"
#include "game/menu_button.h"

void MenuButton_Confirm(MenuButton* button) {
  if (button->onConfirm != NULL) {
    button->onConfirm(button);
  }
}
