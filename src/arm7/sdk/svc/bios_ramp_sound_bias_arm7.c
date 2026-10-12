#include "sdk/svc.h"

/* ARM7 BIOS call 0x08: moves the sound bias level up (raise nonzero) or
   down (raise 0), one step every delay loop iterations. */
#pragma thumb on
asm void Bios_RampSoundBias_Arm7(int raise, int delay)
{
    swi 0x08
    bx lr
}
#pragma thumb reset
