#include "sdk/wl.h"

/* Counter half-overflow interrupt: adds the hardware counters to the totals
   before they wrap, then clears the per-counter overflow flags
   (W_RXSTAT_OVF_IF in GBATEK) and the cause. */
void WifiIrq_StatsHalfFull(void) {
  WifiStats_Accumulate();
  WIFI_MAC_REGISTER(0x1ac) = 0xffff;
  WIFI_IRQ_PENDING_FLAGS = W_IRQ_COUNT_OVERFLOW;
}
