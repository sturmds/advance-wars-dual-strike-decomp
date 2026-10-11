#include "types.h"
#include "sdk/gx.h"

/* Enables or disables the HBlank interrupt request in DISPSTAT; returns the old setting (0 or 16). */
int GX_HBlankIntr(int enable)
{
    int wasEnabled = *REG_DISPSTAT & 0x10;

    if (enable) {
        *REG_DISPSTAT |= 0x10;
    } else {
        *REG_DISPSTAT &= ~0x10;
    }
    return wasEnabled;
}
