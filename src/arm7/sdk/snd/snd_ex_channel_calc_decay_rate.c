#include "sdk/os_arm7.h"

/* Turns a 0-127 decay or release setting into the amount taken off the envelope level each tick. */
u16 SndExChannel_CalcDecayRate(int setting)
{
    if (setting == 127) {
        return 0xFFFF;
    }
    if (setting == 126) {
        return 0x3C00;
    }
    if (setting < 50) {
        return setting * 2 + 1;
    }
    /* needed to match: a signed divide goes through the runtime routine by hand */
    return func_arm7_03806d4c(0x1E00, 126 - setting);
}
