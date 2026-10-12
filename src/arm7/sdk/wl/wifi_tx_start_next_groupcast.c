#include "sdk/wl.h"

void WifiTx_PlaceFrameInRam(WlRamFrame *dest, WlTxBuffer *buffer);

// Starts sending the first frame of WlWork.multicastQueue through the LOC3
// slot, unless that slot is still busy. More data is set while others wait.
void WifiTx_StartNextGroupcast(void)
{
    /* needed to match: this declaration order */
    WlWork *work;
    WlTxBuffer *buffer;
    WlBufferList *queue;
    WlTxSlot *slot;
    WlRamFrame *dest;
    u16 count;
    u32 irqMask;

    work = WIFI_DRIVER_STATE;
    slot = WIFI_TX_LOCATION_OF(work, WIFI_TXLOC_GROUP_OR_PSPOLL);
    queue = &work->multicastQueue;
    count = queue->count;

    if (count == 0) {
        return;
    }
    irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
    if (slot->busy) {
        Irq_SetEnableBits_Arm7(irqMask);
        return;
    }
    slot->busy = 1;
    slot->count++;
    buffer = (WlTxBuffer *)queue->first;
    dest = slot->frame;
    slot->queued = &buffer->frame;
    WifiTx_PlaceFrameInRam(dest, buffer);
    if (count > 1) {
        dest->frameControl |= WIFI_FRAMECTL_MORE_BUFFERED;
    }
    REG_W_TXBUF_LOC3 = W_TXBUF_ENABLE | (((u32)dest & 0x3fff) >> 1);
    Irq_SetEnableBits_Arm7(irqMask);
}
