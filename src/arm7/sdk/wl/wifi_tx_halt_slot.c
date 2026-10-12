#include "sdk/wl.h"

extern const u16 gWlTxSlotResetBits[]; /* REG_W_TXBUF_RESET bit of slots LOC1, LOC2, LOC3 */

/*
 * Stops a TX slot. A frame it was sending keeps the sequence number the
 * hardware gave it (and, unless protected, the +4 halfword), so it can go again.
 */
void WifiTx_HaltSlot(int index)
{
    WlTxSlot *slot = &WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[index];
    OSIrqMask irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

    REG_W_TXBUF_RESET = gWlTxSlotResetBits[index];
    if (slot->busy) {
        if (!(slot->queued->frameControl & WIFI_FRAMECTL_WEP_ON)) {
            slot->queued->unk_0c = slot->frame->unk_04;
        }
        slot->queued->sequence = slot->frame->sequence;
    }
    Irq_SetEnableBits_Arm7(irqMask);
}
