#include "sdk/wl.h"

void WifiTx_HaltAndSettle(void);
void func_arm7_027d0ee8(WlRamFrame *frame, u8 *block);

/* When the queue is busy: stops (WifiTx_HaltAndSettle), passes its frame to
   func_arm7_027d0ee8 again and counts the restart in state.unk_d0. */
void WifiTx_ResendQueueHead(WlTxQueue *queue)
{
    if (queue->busy != 0) {
        WifiTx_HaltAndSettle();
        func_arm7_027d0ee8(queue->frame, queue->unk_0c - 16);
        WIFI_DRIVER_STATE->state.unk_d0++;
    }
}
