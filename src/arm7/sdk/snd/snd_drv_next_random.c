#include "types.h"

extern u32 gSndRandomSeed;

/* Linear congruential generator; returns the high 16 bits. */
u16 SndDrv_NextRandom(void)
{
    gSndRandomSeed = gSndRandomSeed * 0x19660D + 0x3C6EF35F;
    return gSndRandomSeed >> 16;
}
