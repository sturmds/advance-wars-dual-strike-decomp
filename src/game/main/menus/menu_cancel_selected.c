#include "types.h"
#include "game/menu_button.h"

void Menu_CancelSelected(void) {
  MenuButton* button;

  if (MenuCursor_HasSelection() == FALSE) {
    return;
  }
  button = gMenuCursor->selected;
  if (button->onCancel != NULL) {
    button->onCancel(button);
  }
}
