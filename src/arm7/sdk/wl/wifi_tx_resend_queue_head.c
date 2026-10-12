#include "sdk/wl.h"

void func_arm7_027c8240(void);
void func_arm7_027d0ee8(WlRamFrame *frame, u8 *block);

/* When the queue is busy: stops (func_arm7_027c8240), passes its frame to
   func_arm7_027d0ee8 again and counts the restart in state.unk_d0. */
void WifiTx_ResendQueueHead(WlTxQueue *queue)
{
    if (queue->busy != 0) {
        func_arm7_027c8240();
        func_arm7_027d0ee8(queue->frame, queue->unk_0c - 16);
        WIFI_DRIVER_STATE->state.unk_d0++;
    }
}
