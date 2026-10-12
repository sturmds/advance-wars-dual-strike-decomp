#include "sdk/wl.h"

int WifiParam_SetAntennaChoice(int bit3)
{
    if ((u32)bit3 > 1) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.flags.bit3 = (u16)bit3;
    WIFI_ANTENNA_SELECT = WIFI_DRIVER_STATE->state.flags.bit5 ^ WIFI_DRIVER_STATE->state.flags.bit3;
    return 0;
}
