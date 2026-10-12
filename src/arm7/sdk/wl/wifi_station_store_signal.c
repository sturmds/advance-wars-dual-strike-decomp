#include "sdk/wl.h"

void WifiStation_StoreSignal(u16 station, u16 rssi)
{
    WifiStation_Entry(station)->rssi = rssi;
}
