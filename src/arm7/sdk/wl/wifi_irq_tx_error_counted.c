#include "sdk/wl.h"

/* Transmit error counter interrupt. */
void WifiIrq_TxErrorCounted(void)
{
    u32 i;

    WIFI_IRQ_PENDING_FLAGS = W_IRQ_TX_COUNT;
    if (WIFI_DRIVER_STATE->state.flags.bit4) {
        if (!(REG_W_RF_PINS & W_RF_PINS_CARRIER_SENSE)) {
            WIFI_ANTENNA_SELECT ^= 1;
        }
    }
    if (WIFI_DRIVER_STATE->unk_638 & 8) {
        WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
        WlTxSlotView *mp = WIFI_TX_SLOT_RECORDS_OF(WIFI_DRIVER_STATE);

        /* A protected frame whose last two halfwords before the FCS are
           zero: restart the WEP engine. */
        for (i = 0; i < 3; i++) {
            WlMpReplySlot *slot = &mp->txSlots[i];

            if (slot->busy != 0) {
                WlRamFrame *frame = slot->frame;

                if ((frame->frameControl & WIFI_FRAMECTL_WEP_ON) && (frame->unk_04 & 0xff)) {
                    u16 *tail = (u16 *)(((u32)&frame->frameControl + frame->length - 7) & ~1);

                    if (tail[0] == 0 && tail[1] == 0) {
                        frame->unk_04 = 0;
                        REG_W_WEP_CNT = 0;
                        REG_W_WEP_CNT = 0x8000;
                        link->wepRestarts++;
                    }
                }
            }
        }
    }
}
