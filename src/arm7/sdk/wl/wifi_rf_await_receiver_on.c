#include "types.h"
#include "sdk/wl.h"

#define WIFI_RECEIVER_ON_POLLS 4000

/* Polls until the RF unit reports RX on, at most WIFI_RECEIVER_ON_POLLS times. */
void WifiRf_AwaitReceiverOn(void) {
  u32 tries;

  for (tries = WIFI_RECEIVER_ON_POLLS; tries != 0; tries--) {
    if (REG_W_RF_PINS & W_RF_PINS_RX_ON) {
      return;
    }
  }
}
