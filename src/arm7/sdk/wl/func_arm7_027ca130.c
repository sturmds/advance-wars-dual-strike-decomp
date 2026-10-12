#include "sdk/wl.h"

int func_arm7_027ca130(u32 value)
{
    if (value > 3) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.unk_0a = value;
    return 0;
}
