#include "types.h"
#include "sdk/gx.h"

/* 0 clears the register; 1 to 16 fades towards white (bit 14); -1 to -16 fades towards black (bit 15). */
void GXx_SetMasterBrightness_(volatile u16 *reg, int brightness)
{
    if (brightness == 0) {
        *reg = 0;
        return;
    }
    if (brightness > 0) {
        *reg = brightness | 0x4000;
    } else {
        *reg = -brightness | 0x8000;
    }
}
