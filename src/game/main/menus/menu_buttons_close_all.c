#include "types.h"
#include "game/menu_button.h"

/* Starts closing every active button that is not closing yet. */
void MenuButtons_CloseAll(void) {
  int i;
  MenuButton* button = gMenuButtons;

  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    if (button->active && button->slideState < MENU_BUTTON_SLIDE_OUT_START) {
      button->slideState = MENU_BUTTON_SLIDE_OUT_START;
    }
  }
}
