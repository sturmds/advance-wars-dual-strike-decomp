#include "sdk/wl.h"

void WifiTx_HaltAndSettle(void);
void WifiTx_PlaceFrameInRam(WlRamFrame* frame, u8* block);

/* When the queue is busy: stops (WifiTx_HaltAndSettle), passes its frame to
   WifiTx_PlaceFrameInRam again and counts the restart in state.unk_d0. */
void WifiTx_ResendQueueHead(WlTxQueue* queue) {
  if (queue->busy != 0) {
    WifiTx_HaltAndSettle();
    WifiTx_PlaceFrameInRam(queue->frame, queue->unk_0c - 16);
    WIFI_DRIVER_STATE->state.unk_d0++;
  }
}
