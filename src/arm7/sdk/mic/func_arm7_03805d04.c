#include "sdk/mic.h"

/*
 * Sets timer 3 to fire every span bus cycles, picking the smallest prescaler
 * (1, 64, 256 or 1024) that fits. Returns FALSE when span needs more than
 * 16 bits even at 1024.
 */
BOOL func_arm7_03805d04(u32 span) {
  if (span < 0x10000) {
    data_arm7_0380af74.timerControl = TIMER_PRESCALER_1;
    data_arm7_0380af74.timerReload = 0x10000 - span;
    return TRUE;
  }
  if (span < 0x10000 * 64) {
    data_arm7_0380af74.timerControl = TIMER_PRESCALER_64;
    data_arm7_0380af74.timerReload = 0x10000 - (span >> 6);
    return TRUE;
  }
  if (span < 0x10000 * 256) {
    data_arm7_0380af74.timerControl = TIMER_PRESCALER_256;
    data_arm7_0380af74.timerReload = 0x10000 - (span >> 8);
    return TRUE;
  }
  if (span < 0x10000 * 1024) {
    data_arm7_0380af74.timerControl = TIMER_PRESCALER_1024;
    data_arm7_0380af74.timerReload = 0x10000 - (span >> 10);
    return TRUE;
  }
  return FALSE;
}
