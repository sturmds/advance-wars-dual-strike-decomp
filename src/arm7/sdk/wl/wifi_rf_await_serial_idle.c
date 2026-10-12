#include "sdk/wl.h"

#define WIFI_RF_BUSY_POLLS 0x2800

/* Waits for the RF serial interface to go idle; returns TRUE if it timed out. */
BOOL WifiRf_AwaitSerialIdle(void) {
  u32 i;

  for (i = 0; i < WIFI_RF_BUSY_POLLS; i++) {
    if (!(REG_W_RF_BUSY & 1)) {
      return FALSE;
    }
  }
  return TRUE;
}
