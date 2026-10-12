#include "sdk/wl.h"

void func_arm7_027c9560(void);

int WifiParam_StoreRateMode(u32 value)
{
    if (value > 2) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.unk_04 = value;
    func_arm7_027c9560();
    return 0;
}
