#include "types.h"
#include "sdk/gx.h"

/* Enables or disables the VBlank interrupt request in DISPSTAT; returns the old setting (0 or 8). */
int GX_VBlankIntr(int enable)
{
    int wasEnabled = *REG_DISPSTAT & 8;

    if (enable) {
        *REG_DISPSTAT |= 8;
    } else {
        *REG_DISPSTAT &= ~8;
    }
    return wasEnabled;
}
