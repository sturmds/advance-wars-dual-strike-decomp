#include "types.h"
#include "game/touch.h"

/* Called each frame the screen is not touched. If a touch was in progress,
 * ends it: keeps the last position in endX and endY and the phase it ended in
 * in prevPhase. Otherwise clears the active flag. */
void Touch_Release(void) {
  switch (gTouch.phase) {
    case TOUCH_RELEASED:
      break;
    case TOUCH_PRESSED:
    case TOUCH_HELD:
      gTouch.endX = gTouch.x;
      gTouch.endY = gTouch.y;
      gTouch.prevPhase = gTouch.phase;
      gTouch.phase = TOUCH_RELEASED;
      gTouch.active = 1;
      return;
  }
  if (gTouch.active == 1) {
    gTouch.active = 0;
  }
}
