#include "sdk/wl.h"

/*
 * Puts the MP reply that WifiIrq_RxBegun took back into REG_W_TXBUF_REPLY2 again: the
 * hardware copies REPLY1 into REPLY2, so REPLY1 holds the saved reply for that copy
 * and gets its own value back afterwards.
 */
void WifiMp_PutBackReply(void)
{
    WlTxSlots *view;
    u16 reply1;

    view = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE);
    if (view->savedMpReply != 0xffff) {
        reply1 = REG_W_TXBUF_REPLY1;
        REG_W_TXBUF_REPLY1 = view->savedMpReply;
        REG_W_RXCNT |= W_RXCNT_COPY_REPLY;
        REG_W_TXBUF_REPLY1 = reply1;
        view->savedMpReply = 0xffff;
    }
}
