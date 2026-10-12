#include "types.h"
#include "sdk/wl.h"

#define WIFI_BASEBAND_BUSY_POLLS 0x2800

/* Polls until the baseband serial port is free. 0 when it is, 1 after WIFI_BASEBAND_BUSY_POLLS polls. */
int WifiBaseband_AwaitReady(void) {
  u32 i;

  for (i = 0; i < WIFI_BASEBAND_BUSY_POLLS; i++) {
    if (!(REG_W_BB_BUSY & 1)) {
      return 0;
    }
  }
  return 1;
}
