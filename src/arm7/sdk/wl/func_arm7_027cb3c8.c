#include "sdk/wl.h"

void func_arm7_027cb3c8(u16 station, u16 rssi)
{
    WifiStation_Entry(station)->rssi = rssi;
}
