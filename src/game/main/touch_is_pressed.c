#include "types.h"
#include "game/touch.h"

/* TRUE when the touch screen is in the pressed phase and active. */
BOOL Touch_IsPressed(void) {
  if (gTouch.phase == TOUCH_PRESSED && gTouch.active == 1) {
    return TRUE;
  }
  return FALSE;
}
