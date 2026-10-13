#include "types.h"
#include "game/touch.h"
#include "sdk/tp.h"

/* The touch screen calibration from the user settings. */
extern TPCalibrateParam gTouchDriverState;

void Touch_Init(void) {
  TP_Init();
  if (TP_GetUserInfo(&gTouchDriverState)) {
    TP_SetCalibrateParam(&gTouchDriverState);
  }
  gTouch.phase = TOUCH_RELEASED;
  gTouch.prevPhase = 0;
  gTouch.active = 0;
  gTouch.pressFrames = 0;
  gTouch.holdDelay = 20;
}
