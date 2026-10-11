#include "types.h"
#include "sdk/gx.h"

void GX_SetGraphicsMode(int dispMode, int bgMode, int bg0As3d)
{
    u32 dispcnt = *REG_DISPCNT;
    u16 on = gGxDispOn;

    gGxDispMode = dispMode;
    if (on == 0) {
        dispMode = 0;
    }
    *REG_DISPCNT = (dispcnt & 0xfff0fff0) | (dispMode << 16) | bgMode | (bg0As3d << 3);
    if (gGxDispMode == 0) {
        gGxDispOn = 0;
    }
}
