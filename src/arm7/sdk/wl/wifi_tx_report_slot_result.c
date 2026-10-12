#include "sdk/wl.h"

/*
 * Gives a busy slot's queued frame the status the hardware wrote (0, still
 * unsent, becomes WIFI_SEND_RESULT_UNSENT) and calls the slot's onDone.
 */
void WifiTx_ReportSlotResult(int index) {
  WlTxSlot* slot = &WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[index];

  if (slot->busy) {
    if (slot->frame->status == 0) {
      slot->queued->result = WIFI_SEND_RESULT_UNSENT;
    } else {
      slot->queued->result = slot->frame->status;
    }
    slot->onDone(slot->queued, 0);
  }
}
