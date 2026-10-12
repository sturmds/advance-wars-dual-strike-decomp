#include "sdk/wl.h"

// TX done callback of the LOC3 slot, which gets the frame in WIFI_PACKET_RAM: adds the
// TX header results to the counters, then frees the slot.
void WifiTx_Loc3SlotDone(WlRamFrame* frame) {
  u32* counters = WIFI_DRIVER_STATE->counters;

  counters[2] += frame->unk_04 & 0xff;
  if (frame->status & 2) {
    counters[1]++;
  } else {
    counters[0]++;
    counters[4]++;
  }
  WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)
      ->slot[WIFI_TXLOC_GROUP_OR_PSPOLL]
      .busy = 0;
}
