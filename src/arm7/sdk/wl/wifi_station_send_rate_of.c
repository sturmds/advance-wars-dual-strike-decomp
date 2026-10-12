#include "sdk/wl.h"

u16 WifiStation_SendRateOf(u32 station)
{
    if (WifiStation_Entry(station)->rates & 2) {
        return WIFI_RATE_CODE_2M;
    }
    return WIFI_RATE_CODE_1M;
}
