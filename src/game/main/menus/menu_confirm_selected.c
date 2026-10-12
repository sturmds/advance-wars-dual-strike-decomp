#include "types.h"
#include "game/menu_button.h"

void Menu_ConfirmSelected(void) {
  MenuButton* button;

  if (MenuCursor_HasSelection() == FALSE) {
    return;
  }
  button = gMenuCursor->selected;
  if (button->onConfirm != NULL) {
    button->onConfirm(button);
  }
}
