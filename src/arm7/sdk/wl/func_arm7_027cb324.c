#include "sdk/wl.h"


void func_arm7_027cb324(u32 station)
{
    if (WifiStation_StateOf(station) == WIFI_PEER_ASSOCIATED) {
        WIFI_DRIVER_STATE->stationBits.unk_04 &= ~(1 << station);
    }
}
