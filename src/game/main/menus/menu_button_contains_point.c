#include "types.h"
#include "game/menu_button.h"

int MenuButton_TestFlags(MenuButton* button, int mask);

/* Returns TRUE if the point (x, y) is inside the button. */
BOOL MenuButton_ContainsPoint(MenuButton* button, int x, int y) {
  if (MenuButton_TestFlags(button, MENU_BUTTON_TOUCH_ANYWHERE)) {
    return TRUE;
  }
  if (x < button->x || button->x + button->width - 1 < x) {
    return FALSE;
  }
  if (y < button->y || button->y + button->height - 1 < y) {
    return FALSE;
  }
  return TRUE;
}
