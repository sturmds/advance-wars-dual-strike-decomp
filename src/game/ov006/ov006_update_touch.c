#include "types.h"
#include "game/touch.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

#define OV6 data_ov006_0236d640

/* Runs first in each frame of overlay 6. While the screen is touched it sets
 * 0x8000 in OV6->touch (and, for request 1, keeps the touch position). When
 * the touch ends after such a frame, it stores gTouch.prevPhase (1 or 2) with
 * the position; otherwise it clears the field. */
void Ov006_UpdateTouch(void) {
  u8 phase = gTouch.phase;
  u8 x = gTouch.x;
  u8 y = gTouch.y;

  if (phase == TOUCH_RELEASED) {
    u8 kind = gTouch.prevPhase;

    if (kind == 2 && (OV6->touch & 0x8000)) {
      OV6->touch = 2;
      OV6->touchX = x;
      OV6->touchY = y;
      return;
    }
    if (kind == 1 && (OV6->touch & 0x8000)) {
      OV6->touch = 1;
      OV6->touchX = x;
      OV6->touchY = y;
      return;
    }
    OV6->touch = 0;
  } else {
    OV6->touch |= 0x8000;
    if (OV6->request == 1) {
      OV6->touchX = x;
      OV6->touchY = y;
    }
  }
}
