#include "sdk/wl.h"

int func_arm7_027c9e50(int value)
{
    if ((u32)value > 1) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.unk_06 = value;
    return 0;
}
