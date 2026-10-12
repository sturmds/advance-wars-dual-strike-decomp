#include "sdk/wl.h"

/* Counts a sent frame (first try, or how many retries it took) and frees its slot. */
void WifiTx_TallySentFrame(WlTxSlot *slot)
{
    u32 retries = slot->frame->unk_04 & 0xff;
    WlCounters *counters = (WlCounters *)WIFI_DRIVER_STATE->counters;

    if (retries == 0) {
        counters->sentFirstTry++;
    } else {
        counters->retries += retries;
    }
    slot->sentCount++;
    slot->busy = 0;
}
