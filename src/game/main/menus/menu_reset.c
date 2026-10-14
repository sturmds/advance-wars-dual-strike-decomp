#include "types.h"
#include "game/menu_button.h"

/* Deactivates every menu button and resets the cursor: sprite tile 0x3b8,
 * palette 15, nothing selected, screen 0. */
void Menu_Reset(void) {
  int i;
  MenuButton* button;

  button = gMenuButtons;
  for (i = 0; i < MENU_BUTTON_COUNT; i++, button++) {
    button->active = 0;
  }
  gMenuCursor->tile = 0x3b8;
  gMenuCursor->palette = 15;
  gMenuCursor->selected = NULL;
  gMenuCursor->selectedId = 0;
  gMenuCursor->screen = 0;
}
