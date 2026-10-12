#include "types.h"
#include "game/menu_button.h"

void MenuButton_SetTouchState(MenuButton* button, int state) {
  button->touchState = state;
}
