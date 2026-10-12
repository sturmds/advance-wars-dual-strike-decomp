#include "sdk/wl.h"

void func_arm7_027cb300(u32 station)
{
    WIFI_DRIVER_STATE->stationBits.unk_04 |= 1 << station;
}
