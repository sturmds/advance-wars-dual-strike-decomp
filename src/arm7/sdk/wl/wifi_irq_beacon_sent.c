#include "sdk/wl.h"

void WL_WriteByte(void *addr, u32 value);
int func_arm7_027c94c0(int value);
void WifiTx_PauseDataSlots(void);

/* Beacon interrupt. Mode 1 writes WIFI_SHARED_GAME_WORD into the Nintendo element
   of the beacon frame; modes 2 and 3 set up the wakeup for the next beacon and
   drop TX slot frames that waited longer than WlLink.unk_8c periodic ticks. */
void WifiIrq_BeaconSent(void)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    WlStateView *view = WIFI_FROM_STATION_TABLE_OF(WIFI_DRIVER_STATE);
    WlMpReplyView *mp = WIFI_REPLY_SLOTS_VIEW_OF(WIFI_DRIVER_STATE);
    u32 i;
    BOOL wakeForNextBeacon;

    WIFI_IRQ_PENDING_FLAGS = W_IRQ_BEACON;
    switch (link->unk_0c) {
    case 1: {
        /* needed to match: the 8 is added after the offset, not folded into it */
        u8 *body = mp->txFrame->payload + link->gameInfoOffset;
        u8 *addr = body + 8;
        u32 word = WIFI_SHARED_GAME_WORD;
        u16 busy;

        WL_WriteByte(addr, word & 0xff);
        WL_WriteByte(addr + 1, (word >> 8) & 0xff);
        if (link->unk_0e == 1) {
            WIFI_POST_BEACON_TIMER = view->state.unk_10 + WIFI_POST_BEACON_TIMER + 1;
        }
        WIFI_DRIVER_STATE->stationBits.unk_04 = ~WIFI_DRIVER_STATE->stationBits.unk_02 | WIFI_DRIVER_STATE->stationBits.unk_06;
        busy = WIFI_MAC_REGISTER(0x0b6);
        if ((busy & 0x18) || (busy & 6) == 2) {
            mp->unk_8c &= ~2;
            WifiTx_PauseDataSlots();
        } else {
            mp->unk_8c |= 2;
        }
        return;
    }
    case 2:
        if (link->unk_12 == 0) {
            WIFI_POST_BEACON_TIMER = 0xffff;
        } else {
            WIFI_POST_BEACON_TIMER = view->state.unk_10 + WIFI_POST_BEACON_TIMER + 1;
        }
        if (link->unk_1a == 2) {
            func_arm7_027c94c0(2);
        }
        /* fall through */
    case 3:
        break;
    default:
        return;
    }

    if (link->mode != 64) {
        wakeForNextBeacon = TRUE;
    } else {
        wakeForNextBeacon = FALSE;
        if (link->unk_72 == 1) {
            wakeForNextBeacon = TRUE;
        }
        if (link->unk_14 != 0
            && (link->unk_76 == 1 || (link->unk_76 == 0 && link->unk_74 == 1))) {
            wakeForNextBeacon = TRUE;
        }
    }
    if (wakeForNextBeacon) {
        WIFI_MAC_WAKE_CONTROL |= 1; /* W_POWER_TX: wake up for the next beacon */
    } else {
        WIFI_MAC_WAKE_CONTROL &= ~1;
    }
    if (WIFI_MAC_REGISTER(0x118) > 10) {
        WIFI_MAC_POWER_AUX = 0;
    }
    link->unk_72--;
    if (link->unk_72 == 0) {
        link->unk_72 = link->listenInterval;
    }
    if (link->unk_76-- == 0) {
        link->unk_76 = link->unk_74 - 1;
    }

    for (i = 0; i < 2; i++) {
        WlMpReplySlot *slot = &mp->txSlots[i];

        if (slot->busy != 0 && slot->txFrame->result == 0
            && (u16)(link->periodicCount - slot->txFrame->unk_04) > link->unk_8c) {
            WifiTx_HaltSlot(i);
            slot->frame->status = 2;
            WifiTask_Schedule(0, 14);
            mp->txTimeouts++;
        }
    }
    REG_W_TXREQ_SET = 13;
}
