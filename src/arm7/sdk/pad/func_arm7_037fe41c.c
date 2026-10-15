#include "types.h"
#include "sdk/pad.h"
#include "sdk/rtc.h"

/* Alarm callback: puts the port in general-purpose mode, then copies the X,
 * Y and debug buttons and the lid state from EXTKEYIN to the halfword the
 * ARM9 reads. */
void func_arm7_037fe41c(void* arg) {
  u16 fold = 0;
  u16 keys;

  func_arm7_037fe3e0(RCNT_MODE_GPIO);
  keys = *REG_EXTKEYIN;
  if (keys & EXTKEYIN_FOLD) {
    fold = EXTRA_KEYS_LID_CLOSED;
  }
  *EXTRA_KEYS_MIRROR =
      fold | ((keys & (EXTKEYIN_X | EXTKEYIN_Y | EXTKEYIN_DEBUG))
              << EXTRA_KEYS_TO_KEYINPUT_SHIFT);
}
