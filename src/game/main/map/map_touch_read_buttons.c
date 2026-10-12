#include "types.h"
#include "game/touch.h"

BOOL MapTouch_InLeftButton(int x, int y);
BOOL MapTouch_InRightButton(int x, int y);
BOOL MapTouch_InBackButton(int x, int y);

extern s16 gEventScriptOwnsInput;

/* What MapTouch_ReadButtons returns. */
#define TAP_NONE 0
#define TAP_CENTER_BUTTON 0x10 /* button at x 112-144, y 0-16 */
#define TAP_MAP 0x20
#define TAP_LEFT_BUTTON \
  0x40 /* button at x 80-112; the map screen runs SceneMode_OnButtonX */
#define TAP_RIGHT_BUTTON 0x100 /* button at x 144-176 */

static inline BOOL IsTouchInPhase(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* Sorts a touch on the map screen: one of the three buttons along the top
 * edge, or the map. The right and left buttons count only while
 * `rightButton` and `leftButton` are set. A touch that did not begin this
 * frame counts as on the map. Returns TAP_NONE with no touch or while
 * gEventScriptOwnsInput is set. */
u32 MapTouch_ReadButtons(int rightButton, int leftButton) {
  int touchX = gTouch.x;
  int touchY = gTouch.y;

  if (gEventScriptOwnsInput != 0) {
    return TAP_NONE;
  }
  if (gTouch.phase == TOUCH_RELEASED) {
    return TAP_NONE;
  }
  if (!IsTouchInPhase(TOUCH_PRESSED)) {
    return TAP_MAP;
  }
  if (MapTouch_InBackButton(touchX, touchY)) {
    return TAP_CENTER_BUTTON;
  }
  if (rightButton && MapTouch_InRightButton(touchX, touchY)) {
    return TAP_RIGHT_BUTTON;
  }
  if (leftButton && MapTouch_InLeftButton(touchX, touchY)) {
    return TAP_LEFT_BUTTON;
  }
  return TAP_MAP;
}
