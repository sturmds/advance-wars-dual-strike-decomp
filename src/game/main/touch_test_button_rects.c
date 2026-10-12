#include "types.h"
#include "game/rect.h"
#include "game/touch.h"

u32 Rects_FindHitBit(int x, int y, const Rect* rects, int count);

/* Button bit of the rect pressed at the start of the current touch. */
extern u32 gTouchPressedButton;

#define TOUCH_RECT_RELEASED_ON_PRESS 0x10

/* Tests the touch against count rects. Returns the button bit of the rect
 * under the touch, or TOUCH_RECT_RELEASED_ON_PRESS when
 * the touch ends on the rect where it started. */
u16 Touch_TestButtonRects(const Rect* rects, int count) {
  u32 hit = Rects_FindHitBit(gTouch.x, gTouch.y, rects, count);
  int phase = gTouch.phase;

  switch (phase) {
    case TOUCH_HELD: /* needed to match, though default covers it */
    default:
      if (Touch_InPhase(phase, TOUCH_HELD)) {
        gTouchPressedButton = 0;
      }
      /* fall through */
    case TOUCH_PRESSED:
      if (Touch_InPhase(phase, TOUCH_PRESSED)) {
        gTouchPressedButton = hit;
      } else if (gTouchPressedButton != hit) {
        gTouchPressedButton = 0;
      }
      if (hit != 0) {
        return hit;
      }
      break;
    case TOUCH_RELEASED:
      if (Touch_InPhase(phase, TOUCH_RELEASED) && gTouchPressedButton != 0 &&
          gTouchPressedButton == hit) {
        return TOUCH_RECT_RELEASED_ON_PRESS;
      }
      break;
  }
  return 0;
}
