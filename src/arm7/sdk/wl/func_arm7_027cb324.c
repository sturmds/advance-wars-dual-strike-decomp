#include "sdk/wl.h"

u16 func_arm7_027cb140(u16 station);

void func_arm7_027cb324(u16 station)
{
    if (func_arm7_027cb140(station) == WIFI_PEER_ASSOCIATED) {
        WIFI_DRIVER_STATE->stationBits.unk_04 &= ~(1 << station);
    }
}
