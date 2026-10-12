#include "sdk/wl.h"

void func_arm7_027d0424(void);
void WifiStation_ChangeState(u16 station, u16 state);

void WifiLink_DropAssocId(void)
{
    WlParams *params = WIFI_SETTINGS_BLOCK_OF(WIFI_DRIVER_STATE);

    params->aid = 0;
    WIFI_DRIVER_STATE->aidBit = 0;
    WifiRf_AwaitQuietAir();
    IO_WIRELESS_STATION_ID_4BIT = 0;
    IO_WIRELESS_STATION_ID = 0;
    if (params->unk_88 != 0) {
        func_arm7_027d0424();
        WifiStation_ChangeState(params->unk_88, 32);
        params->unk_88 = 0;
    }
}
