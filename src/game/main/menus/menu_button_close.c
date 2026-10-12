#include "types.h"
#include "game/menu_button.h"

/* Starts closing the active button with this id, if it is not closing yet. */
void MenuButton_Close(int id) {
  MenuButton* button = MenuButton_Find(id);

  if (button == NULL) {
    return;
  }
  if (button->slideState < MENU_BUTTON_SLIDE_OUT_START) {
    button->slideState = MENU_BUTTON_SLIDE_OUT_START;
  }
}
