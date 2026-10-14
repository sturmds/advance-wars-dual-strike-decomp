#include "types.h"
#include "game/menu_button.h"

MenuCursor* gMenuCursor = &data_0229b38c;

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
