#include "sdk/wl.h"

void WifiTask_DrainRxRing(void);
void func_arm7_027c92e8(void);

/* The MP poll the CMD slot sent, read again at each use as the original does. */
#define WIFI_SENT_POLL_OF(slots) (&((WlMpCommand *)(slots)->cmdBuffer)->poll)

// Task 16, also run by WifiTx_AbortCmdSlot: finishes the CMD slot. Runs
// WifiTask_DrainRxRing when the hardware's AID word in the TX header differs
// from the poll's pendingAids, adds the retry count to counters[25] (at least
// 1), records which AIDs changed, frees the slot and hands the poll buffer
// back to the host.
void WifiTask_FinishMpPoll(void)
{
    WlTxSlots *slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE);
    u32 retries;

    if (slots->slot[WIFI_TXLOC_MP_POLL].busy == 0) {
        return;
    }
    if (WIFI_SENT_POLL_OF(slots)->pendingAids != slots->slot[WIFI_TXLOC_MP_POLL].frame->unk_02) {
        WifiTask_DrainRxRing();
    }
    retries = slots->slot[WIFI_TXLOC_MP_POLL].frame->unk_04 & 0xff;
    if (retries != 0) {
        WIFI_DRIVER_STATE->counters[25] += retries;
    } else {
        WIFI_DRIVER_STATE->counters[25]++;
    }
    slots->slot[WIFI_TXLOC_MP_POLL].sentCount++;
    WIFI_SENT_POLL_OF(slots)->unk_02 = WIFI_SENT_POLL_OF(slots)->pendingAids ^ slots->slot[WIFI_TXLOC_MP_POLL].frame->unk_02;
    slots->mpPollBitmapSaved = WIFI_SENT_POLL_OF(slots)->pendingAids;
    slots->slot[WIFI_TXLOC_MP_POLL].busy = 0;
    if (WIFI_DRIVER_STATE->state.unk_be) {
        func_arm7_027c92e8();
    }
    WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, slots->cmdBuffer);
}
