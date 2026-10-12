#include "sdk/wl.h"

void Mem_FillHalfwords_Arm7(u16 data, void *dest, int size);
void WifiFlash_ReadRadioBytes(u32 offset, u32 size, void *dest);

/* Clears the RF settings and reads the RF chip type and entry format from firmware 0x40-0x43. */
void WifiRf_ReadChipFormat(void)
{
    WlRfConfig *rf = &WIFI_DRIVER_STATE->rf;
    /* volatile: needed to match, the original stores the zero to the stack and loads it back. */
    volatile u16 zero = 0;

    Mem_FillHalfwords_Arm7(zero, rf, sizeof(WlRfConfig));
    WifiFlash_ReadRadioBytes(0x40, 1, &rf->type);
    WifiFlash_ReadRadioBytes(0x41, 1, &rf->bitsPerEntry);
    WifiFlash_ReadRadioBytes(0x42, 1, &rf->entryCount);
    WifiFlash_ReadRadioBytes(0x43, 1, &rf->unk_06);
}
