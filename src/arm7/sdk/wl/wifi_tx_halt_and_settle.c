#include "sdk/wl.h"

/* Writes 0 to register 0x004, then polls the RF status up to 16 times until it reads 0 or 9. */
void WifiTx_HaltAndSettle(void) {
  int tries;
  u16 status;

  WIFI_MAC_RUN = 0;
  for (tries = 16; tries != 0; tries--) {
    status = REG_W_RF_STATUS;
    if (status == 0 || status == 9) {
      return;
    }
  }
}
