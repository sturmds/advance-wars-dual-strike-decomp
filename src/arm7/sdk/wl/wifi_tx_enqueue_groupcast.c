#include "sdk/wl.h"

void WifiBeacon_MarkTrafficFor(u16 station);

/* Moves a buffer from a list to the multicast queue. */
void WifiTx_EnqueueGroupcast(WlBufferList *from, WlBuffer *buffer)
{
    WlBufferList *queue = &WIFI_DRIVER_STATE->multicastQueue;
    u32 irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

    if (queue->count == 0) {
        WifiBeacon_MarkTrafficFor(0);
    }
    WifiBuffer_Transfer(from, queue, buffer);
    Irq_SetEnableBits_Arm7(irqMask);
}
