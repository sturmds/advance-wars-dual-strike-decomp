#include "sdk/wl.h"

int func_arm7_027c8b8c(int reg, int value);

int WifiBaseband_SetCcaParams(int reg13Value, int reg35Value, void *request)
{
    if ((u32)reg13Value > 3) {
        return 5;
    }
    if ((u32)reg35Value > 0x3f) {
        return 5;
    }
    func_arm7_027c8b8c(0x13, reg13Value);
    func_arm7_027c8b8c(0x35, reg35Value);
    return 0;
}
