#include "sdk/mi.h"
#include "sdk/wl.h"

/* Empties the BSS list. */
void WifiScan_ForgetNetworks(void) {
  /* needed to match: the original stores the fill value to the stack and reloads it */
  volatile u16 zero = 0;

  Mem_FillHalfwords_Arm7(zero, WIFI_DRIVER_STATE->limits.bssList,
                         sizeof(WIFI_DRIVER_STATE->limits.bssList));
}
