#include "types.h"
#include "game/keypad.h"
#include "game/touch.h"

u32 Link_IsActive(void);
void func_020526e0(void);

/* Script step of the day banner: A, B, Start or a new touch starts the
 * early-exit script (func_020526e0). Does nothing in the link mode that
 * Link_IsActive reports. */
void DayBanner_CheckSkip(void) {
  if (Link_IsActive() == 1) {
    return;
  }
  if (!(gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))) {
    if (gTouch.phase != TOUCH_PRESSED) {
      return;
    }
    if (gTouch.active != 1) {
      return;
    }
  }
  func_020526e0();
}
