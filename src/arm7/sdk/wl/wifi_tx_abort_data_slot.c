#include "sdk/wl.h"

/*
 * Stops the LOC1 slot, and in mode 1 the LOC3 (multicast) slot too; the frame
 * each was sending and the rest of its queue go back to the host.
 */
void WifiTx_AbortDataSlot(void) {
  WlTxSlot* slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot;
  OSIrqMask irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

  if (WIFI_DRIVER_STATE->state.unk_24 == 1) {
    REG_W_TXBUF_RESET = W_TXREQ_LOC1 | W_TXREQ_LOC3;
    if (slots[WIFI_TXLOC_GROUP_OR_PSPOLL].busy) {
      WifiTx_ReportSlotResult(WIFI_TXLOC_GROUP_OR_PSPOLL);
    }
    WifiTx_DrainQueue(WIFI_SEND_QUEUE_GROUP, TRUE);
  } else {
    REG_W_TXBUF_RESET = W_TXREQ_LOC1;
  }
  if (slots[WIFI_TXLOC_DATA].busy) {
    WifiTx_ReportSlotResult(WIFI_TXLOC_DATA);
  }
  WifiTx_DrainQueue(0, TRUE);
  Irq_SetEnableBits_Arm7(irqMask);
}
