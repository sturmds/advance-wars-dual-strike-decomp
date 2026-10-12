#include "sdk/wl.h"

int WifiParam_SetAntennaSwitching(int bit4, int bit5)
{
    if ((u32)bit4 > 1 || (u32)bit5 > 1) {
        return 5;
    }
    switch (bit4) {
    case 0:
        WIFI_DRIVER_STATE->state.flags.bit5 = (u16)bit5;
        break;
    case 1:
        if (WIFI_DRIVER_STATE->state.phase != 1) {
            return 11;
        }
        WIFI_DRIVER_STATE->state.flags.bit5 = 0;
        break;
    }
    WIFI_DRIVER_STATE->state.flags.bit4 = (u16)bit4;
    WIFI_ANTENNA_SELECT = WIFI_DRIVER_STATE->state.flags.bit5 ^ WIFI_DRIVER_STATE->state.flags.bit3;
    return 0;
}
