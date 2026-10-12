#include "sdk/wl.h"

/* Stops both MP reply TX slots and marks them free. */
void WifiTx_AbortReplySlots(void)
{
    WlTxSlot *slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot;
    OSIrqMask irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

    REG_W_TXBUF_RESET = W_TXBUF_RESET_REPLY1 | W_TXBUF_RESET_REPLY2;
    slots[WIFI_TXLOC_POLL_ANSWER_A].busy = 0;
    slots[WIFI_TXLOC_POLL_ANSWER_B].busy = 0;
    Irq_SetEnableBits_Arm7(irqMask);
}
