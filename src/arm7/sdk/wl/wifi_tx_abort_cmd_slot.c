#include "sdk/wl.h"

void WifiTask_FinishMpPoll(void); /* in a handler table; probably the CMD TX done handler */

/* Stops the CMD TX slot and finishes a frame it was sending. */
void WifiTx_AbortCmdSlot(void)
{
    WlTxSlot *slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot;
    OSIrqMask irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

    REG_W_TXBUF_RESET = W_TXREQ_CMD;
    if (slots[WIFI_TXLOC_MP_POLL].busy) {
        WifiTask_FinishMpPoll();
    }
    Irq_SetEnableBits_Arm7(irqMask);
}
