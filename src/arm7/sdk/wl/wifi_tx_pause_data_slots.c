#include "sdk/wl.h"

// Stops TX slots LOC2 and LOC1. A slot whose frame the hardware has not
// marked done is freed; if either frame was done, task 14 runs its onDone.
// Task 20 then starts sending again.
void WifiTx_PauseDataSlots(void)
{
    WlTxSlots *slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE);
    BOOL done = FALSE;

    WifiTx_HaltSlot(WIFI_TXLOC_MGMT);
    WifiTx_HaltSlot(WIFI_TXLOC_DATA);
    if (slots->slot[WIFI_TXLOC_MGMT].frame->status != 0) {
        done = TRUE;
    } else {
        slots->slot[WIFI_TXLOC_MGMT].busy = 0;
    }
    if (slots->slot[WIFI_TXLOC_DATA].frame->status != 0) {
        done = TRUE;
    } else {
        slots->slot[WIFI_TXLOC_DATA].busy = 0;
    }
    if (done) {
        WifiTask_Schedule(0, 14);
    }
    WifiTask_Schedule(0, 20);
}
