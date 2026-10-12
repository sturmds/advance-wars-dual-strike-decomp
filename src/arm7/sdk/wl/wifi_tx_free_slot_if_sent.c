#include "sdk/wl.h"

void WifiTx_TallySentFrame(WlTxSlot *slot);

/* Releases the slot once the hardware marked its frame sent (status bit 0),
   unless REG_W_TXBUF_REPLY2 still points at that frame. Returns TRUE if it
   did. */
BOOL WifiTx_FreeSlotIfSent(WlTxSlot *slot)
{
    if (slot->busy != 0 && (slot->frame->status & 1) != 0
        && ((((u32)slot->frame & 0x3fff) >> 1) | W_TXBUF_ENABLE) != REG_W_TXBUF_REPLY2) {
        WifiTx_TallySentFrame(slot);
        return TRUE;
    }
    return FALSE;
}
