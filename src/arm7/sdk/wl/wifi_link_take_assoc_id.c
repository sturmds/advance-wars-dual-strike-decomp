#include "sdk/wl.h"

void WifiLink_TakeAssocId(u16 aid)
{
    WIFI_DRIVER_STATE->state.aid = aid;
    WIFI_DRIVER_STATE->aidBit = 1 << aid;
    IO_WIRELESS_STATION_ID = aid;
    IO_WIRELESS_STATION_ID_4BIT = aid;
}
