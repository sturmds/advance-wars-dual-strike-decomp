#include "types.h"
#include "game/menu_button.h"

int MenuButton_TestFlags(MenuButton* button, int mask) {
  return button->flags & mask;
}
