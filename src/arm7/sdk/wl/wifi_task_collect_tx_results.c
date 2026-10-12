#include "sdk/wl.h"

// Task 14: for each of TX slots LOC3-LOC1 that is busy and whose send request
// the hardware has cleared, copies the result back into the queued frame and
// calls the slot's onDone. A WEP frame that went out with an all-zero ICV
// (option bit 3) restarts the WEP engine and is sent again, unless it was
// queued more than link->unk_8c periodic alarms ago.
void WifiTask_CollectTxResults(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlTxSlots *slots = WIFI_TX_LOCATIONS_OF(work);
    int i;

    for (i = WIFI_TXLOC_GROUP_OR_PSPOLL; i >= WIFI_TXLOC_DATA; i--) {
        WlTxSlot *slot = &slots->slot[i];
        WlRamFrame *frame;

        if (*REG_W_TXBUF_LOC(i) & W_TXBUF_ENABLE) {
            continue;
        }
        if (slot->busy == 0) {
            continue;
        }
        /* slot->queued is read again at each use, as the original does */
        if (slot->queued != NULL) {
            slot->queued->result = slot->frame->status;
            slot->queued->frameControl = slot->frame->frameControl;
            frame = slot->frame;
            if (frame->frameControl & WIFI_FRAMECTL_WEP_ON) {
                if (WIFI_DRIVER_STATE->unk_638 & 8) {
                    /* the WEP ICV, just before the FCS */
                    u16 *icv = (u16 *)(((u32)&frame->frameControl + frame->length - 7) & ~1);

                    if (icv[0] == 0 && icv[1] == 0) {
                        u16 age = link->periodicCount - slot->queued->unk_04;

                        REG_W_WEP_CNT = 0;
                        REG_W_WEP_CNT = 0x8000;
                        link->wepRestarts++;
                        if (age > link->unk_8c) {
                            slot->queued->result = WIFI_SEND_RESULT_UNSENT;
                            slot->sentCount++;
                            slots->txTimeouts++;
                            slot->onDone(slot->queued, 1);
                        } else {
                            slot->queued->unk_0c = 0;
                            *REG_W_TXBUF_LOC(i) |= W_TXBUF_ENABLE;
                        }
                        continue;
                    }
                }
                slot->queued->unk_0c += frame->unk_04 & 0xff;
            } else {
                slot->queued->unk_0c = frame->unk_04 & 0xff;
            }
            slot->sentCount++;
            slot->onDone(slot->queued, 2);
        } else {
            slot->sentCount++;
            /* no queued frame: onDone gets the WIFI_PACKET_RAM frame */
            slot->onDone((WlTxFrame *)slot->frame, 3);
        }
    }
}
