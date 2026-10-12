#include "sdk/wl.h"
#include "sdk/mi.h"

void WifiStats_Zero(void) {
  volatile u32
      zero; /* needed to match: the fill value goes through the stack */

  WifiStats_Accumulate();
  zero = 0;
  Mem_FillWords_Arm7(zero, &WIFI_DRIVER_STATE->counters, sizeof(WlCounters));
}
