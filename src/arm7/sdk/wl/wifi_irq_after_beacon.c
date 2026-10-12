#include "sdk/wl.h"

/* Post-beacon interrupt: stops the transmit requests WifiIrq_BeaconSent started and
   steps WlLink.unk_1a from 1 to 2 to 0. */
void WifiIrq_AfterBeacon(void) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);

  WIFI_IRQ_PENDING_FLAGS = W_IRQ_POST_BEACON;
  REG_W_TXREQ_RESET = 13;
  if (link->unk_1a == 1) {
    link->unk_1a = 2;
  } else if (link->unk_1a == 2) {
    link->unk_1a = 0;
  } else if (link->unk_0c == 2 && link->mode != 64) {
    WIFI_MAC_POWER_AUX = 0;
  }
}
