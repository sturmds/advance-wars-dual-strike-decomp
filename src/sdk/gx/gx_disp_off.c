#include "types.h"
#include "sdk/gx.h"

/* Blanks the main display; remembers the mode so GX_DispOn can restore it. */
void GX_DispOff(void)
{
    u32 dispcnt = *REG_DISPCNT;
    u16 dispMode = (dispcnt & 0x30000) >> 16;

    gGxDispOn = 0;
    gGxDispMode = dispMode;
    *REG_DISPCNT = dispcnt & ~0x30000;
}
