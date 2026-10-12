#include "types.h"
#include "game/menu_button.h"
#include "game/touch.h"

int MenuButton_TestFlags(MenuButton* entry, int mask);
BOOL EventScript_IsRunning(void);

/* Needed to match: the original builds this test as a 0/1 value first,
 * as an inline function does. */
static inline BOOL Touch_Is(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

static inline int Abs(int value) { return value < 0 ? -value : value; }

/* Touch handling of a shown button, once per frame. Returns TRUE when the
 * touch confirmed the button: pressed and released on it, with the release
 * at most 16 pixels from where the touch began. */
BOOL MenuButton_HandleTouch(MenuButton* button) {
  u8 x = gTouch.x;
  u8 y = gTouch.y;
  u8 startX = gTouch.startX;
  u8 startY = gTouch.startY;

  switch (MenuButton_GetTouchState(button)) {
    case MENU_BUTTON_TOUCH_HELD: /* needed to match: this case comes first */
      if (MenuButton_ContainsPoint(button, x, y) == FALSE) {
        MenuButton_SetTouchState(button, MENU_BUTTON_TOUCH_NONE);
        break;
      }
      if (Touch_Is(TOUCH_RELEASED)) {
        MenuButton_SetTouchState(button, MENU_BUTTON_TOUCH_NONE);
      }
      break;
    case MENU_BUTTON_TOUCH_NONE:
      if (Touch_Is(TOUCH_PRESSED) &&
          MenuButton_ContainsPoint(button, x, y) == TRUE) {
        MenuButton_SetTouchState(button, MENU_BUTTON_TOUCH_PRESSED);
      }
      break;
    case MENU_BUTTON_TOUCH_PRESSED:
      if (MenuButton_ContainsPoint(button, x, y) == FALSE) {
        MenuButton_SetTouchState(button, MENU_BUTTON_TOUCH_NONE);
        break;
      }
      if (Touch_Is(TOUCH_RELEASED) == TRUE) {
        MenuButton_SetTouchState(button, MENU_BUTTON_TOUCH_NONE);
        if (MenuButton_TestFlags(button, MENU_BUTTON_NO_TOUCH_CONFIRM) != 0) {
          return FALSE;
        }
        if (EventScript_IsRunning() != FALSE) {
          return FALSE;
        }
        if (MenuButton_TestFlags(button, MENU_BUTTON_SELECT_ON_TOUCH) != 0) {
          MenuCursor_SelectButton(button);
        }
        if (Abs(x - startX) > 16 || Abs(y - startY) > 16) {
          return FALSE;
        }
        if (button->onConfirm != NULL) {
          button->onConfirm(button);
        }
        return TRUE;
      }
      if (Touch_Is(TOUCH_HELD)) {
        MenuButton_SetTouchState(button, MENU_BUTTON_TOUCH_HELD);
      }
      break;
  }
  return FALSE;
}
