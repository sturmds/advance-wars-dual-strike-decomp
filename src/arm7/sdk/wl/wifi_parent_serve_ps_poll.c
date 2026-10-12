#include "types.h"
#include "sdk/wl.h"

u16 func_arm7_027cb140(u16 station);   /* station state */
void func_arm7_027cb300(u16 station);  /* sets the station's bit in stationBits.unk_04 */
void func_arm7_027d10f4(int queue);    /* sends what waits in WlWork.txQueues[queue] */

/*
 * PS-Poll from an associated station: marks the station and sends what
 * waits in TX queues 1 and 0.
 */
void WifiParent_ServePsPoll(WlRxFrame *frame)
{
    WlBufferLists *lists = WIFI_LISTS_VIEW_OF(WIFI_DRIVER_STATE);
    u16 station = frame->station;

    if (func_arm7_027cb140(station) == WIFI_PEER_ASSOCIATED) {
        func_arm7_027cb300(station);
        if (lists->txQueues[1].count != 0) {
            func_arm7_027d10f4(1);
        }
        if (lists->txQueues[0].count != 0) {
            func_arm7_027d10f4(0);
        }
    }
}
