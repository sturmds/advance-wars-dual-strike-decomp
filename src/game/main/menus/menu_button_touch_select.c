#include "types.h"
#include "game/menu_button.h"
#include "game/touch.h"

BOOL EventScript_IsRunning(void);
int MenuButton_TestFlags(MenuButton* button, int mask);

/* Selects the button when the touch point is inside it. Returns TRUE if
 * this frame's touch selected it. */
BOOL MenuButton_TouchSelect(MenuButton* button) {
  int x = gTouch.x;
  int y = gTouch.y;

  if (EventScript_IsRunning()) {
    return FALSE;
  }
  if (gTouch.phase == 0) {
    return FALSE;
  }
  if (MenuButton_TestFlags(button, MENU_BUTTON_SELECT_ON_TOUCH) == 0) {
    return FALSE;
  }
  if (!MenuButton_ContainsPoint(button, x, y)) {
    return FALSE;
  }
  if (gMenuCursor->selected == button) {
    return FALSE;
  }
  if (MenuButton_TestFlags(button, MENU_BUTTON_NO_TOUCH_CONFIRM)) {
    return FALSE;
  }
  MenuCursor_SelectButton(button);
  return TRUE;
}
