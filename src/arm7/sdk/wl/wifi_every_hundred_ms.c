#include "sdk/wl.h"

/* Runs every 100 ms (set up by 0x037f9380 through Wifi_ArmPeriodicMs): queues the periodic WL tasks. */
void Wifi_EveryHundredMs(void *arg)
{
    WlWork *work = WIFI_DRIVER_STATE;

    work->state.periodicCount++;
    WifiTask_Schedule(1, 10);
    WifiTask_Schedule(2, 18);
    WifiTask_Schedule(1, 17);
    if (work->outbox.count != 0) {
        WifiTask_Schedule(2, 19);
    }
    if (work->state.unk_c8 != 0) {
        WifiTask_Schedule(2, 21);
    }
}
