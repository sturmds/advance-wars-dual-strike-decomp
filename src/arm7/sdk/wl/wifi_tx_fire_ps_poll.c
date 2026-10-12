#include "sdk/wl.h"

u16 func_arm7_027c8c00(void); /* TX rate for the current mode: WIFI_RATE_CODE_1M or 2MBPS */

/*
 * Sends the PS-Poll frame from the LOC3 slot, unless the slot is still busy.
 * The beacon handler calls it when the TIM has this station's bit set.
 */
void WifiTx_FirePsPoll(void)
{
    WlTxSlot *slot = WIFI_TX_LOCATION_OF(WIFI_DRIVER_STATE, WIFI_TXLOC_GROUP_OR_PSPOLL);

    slot->count++;
    if (slot->busy) {
        slot->frame->unk_04 = 0;
    } else {
        slot->busy = 1;
        slot->frame->status = 0;
        slot->frame->unk_04 = 0;
        slot->frame->rate = func_arm7_027c8c00();
        /* needed to match: the u16 cast (WifiBeacon_Enable has none) */
        REG_W_TXBUF_LOC3 = (u16)(((u32)slot->frame & 0x3fff) >> 1) | W_TXBUF_ENABLE;
    }
}
