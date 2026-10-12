#include "sdk/wl.h"

/* Marks the beacon slot busy and points the beacon TX location at the beacon frame. */
void WifiBeacon_Enable(void)
{
    WlTxSlot *slot = WIFI_BEACON_TX_SLOT_OF(WIFI_DRIVER_STATE);

    slot->busy = 1;
    REG_W_TXBUF_BEACON = (((u32)slot->frame & 0x3fff) >> 1) | W_TXBUF_ENABLE;
}
