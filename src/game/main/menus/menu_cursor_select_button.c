#include "types.h"
#include "game/menu_button.h"

MenuButton* MenuCursor_SelectButton(MenuButton* button) {
  return MenuCursor_SetSelected(button, button->id);
}
