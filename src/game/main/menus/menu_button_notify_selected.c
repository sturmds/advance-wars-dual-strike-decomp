#include "types.h"
#include "game/menu_button.h"

void MenuButton_NotifySelected(MenuButton* button) {
  if (button->onSelect != NULL) {
    button->onSelect(button);
  }
}
