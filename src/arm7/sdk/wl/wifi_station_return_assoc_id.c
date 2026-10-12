#include "sdk/wl.h"

void func_arm7_027c92e8(void);
void WifiBeacon_UnmarkTrafficFor(u16 station);

void WifiStation_ReturnAssocId(u16 station)
{
    WlWork *work = WIFI_DRIVER_STATE;
    u32 aid;

    WifiBeacon_UnmarkTrafficFor(station);
    aid = WifiStation_AssocIdOf(station);
    WifiStation_Entry(station)->aid = 0;
    work->stationBits.aidMask &= ~(1 << aid);
    work->stationBits.aidCount--;
    if (work->stationBits.aidCount == 0) {
        func_arm7_027c92e8();
    }
}
