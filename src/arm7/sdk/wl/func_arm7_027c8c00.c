#include "sdk/wl.h"

int func_arm7_027c8c00(void)
{
    switch (WIFI_DRIVER_STATE->state.unk_04) {
    case 0:
        if (WIFI_DRIVER_STATE->state.unk_78 & 1) {
            return 10;
        }
        break;
    case 1:
        return 10;
    }
    return 20;
}
