#include "sdk/wl.h"

void func_arm7_037fde28(u16 data, void *dest, int size);
void func_arm7_027d422c(u32 offset, u32 size, void *dest);

/* Clears the RF settings and reads the RF chip type and entry format from firmware 0x40-0x43. */
void WifiRf_ReadChipFormat(void)
{
    WlRfConfig *rf = &WIFI_DRIVER_STATE->rf;
    /* volatile: needed to match, the original stores the zero to the stack and loads it back. */
    volatile u16 zero = 0;

    func_arm7_037fde28(zero, rf, sizeof(WlRfConfig));
    func_arm7_027d422c(0x40, 1, &rf->type);
    func_arm7_027d422c(0x41, 1, &rf->bitsPerEntry);
    func_arm7_027d422c(0x42, 1, &rf->entryCount);
    func_arm7_027d422c(0x43, 1, &rf->unk_06);
}
