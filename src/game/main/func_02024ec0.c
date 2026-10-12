#include "types.h"
#include "game/rect.h"
#include "game/touch.h"

void func_020199c0(Rect* rect);

int func_02024ec0(void) {
  Rect screen;
  int x = gTouch.x;
  int y = gTouch.y;
  BOOL pressed;

  func_020199c0(&screen);
  if (gTouch.phase == 0) {
    return 0;
  }
  if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
    pressed = TRUE;
  } else {
    pressed = FALSE;
  }
  if (!pressed) {
    return 0;
  }
  if (screen.x < x && x < screen.x + screen.width && screen.y < y &&
      y < screen.y + screen.height) {
    return 16;
  }
  return 0;
}
