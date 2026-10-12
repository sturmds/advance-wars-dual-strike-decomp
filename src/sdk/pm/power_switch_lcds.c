#include "sdk/gx.h"
#include "sdk/pm.h"

extern u32 data_022ab8f4;

void func_02114ee8(int led);

BOOL Power_SwitchLcds(int on, int led, BOOL force, BOOL sync) {
  switch (on) {
    case 1:
      if (!force && *SHARED_FRAME_COUNTER - data_022ab8f4 <= 7) {
        return FALSE;
      }
      if (led != 0) {
        if (sync) {
          func_02114ee8(led);
        } else {
          Power_RequestLedState(led, NULL, NULL);
        }
      }
      *REG_POWCNT1 |= POWCNT1_LCD;
      break;
    case 0:
      *REG_POWCNT1 &= ~POWCNT1_LCD;
      data_022ab8f4 = *SHARED_FRAME_COUNTER;
      if (led != 0) {
        if (sync) {
          func_02114ee8(led);
        } else {
          Power_RequestLedState(led, NULL, NULL);
        }
      }
      break;
  }
  return TRUE;
}
