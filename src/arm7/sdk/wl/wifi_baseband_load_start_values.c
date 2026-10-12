#include "sdk/wl.h"

void func_arm7_027d422c(u32 offset, u32 size, void *dest);
int func_arm7_027c8b8c(u32 index, u32 value);

/* Firmware 0x64-0xcc holds the start values of the 105 baseband registers. */
void WifiBaseband_LoadStartValues(void)
{
    u32 i;
    u32 offset;
    u32 value;

    WIFI_MAC_REGISTER(0x160) = 0x100;
    /* Needed to match: the three set in this order, outside the for. */
    offset = 0x64;
    i = 0;
    value = 0;
    for (; i < 105; offset++, i++) {
        func_arm7_027d422c(offset, 1, &value);
        func_arm7_027c8b8c(i, value);
    }
}
