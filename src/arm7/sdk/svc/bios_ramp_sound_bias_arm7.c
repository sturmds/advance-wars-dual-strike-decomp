#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x08: moves the sound bias level up (raise nonzero) or
   down (raise 0), one step every delay loop iterations. */
#pragma thumb on
asm void Bios_RampSoundBias_Arm7(int raise, int delay)
{
    swi 0x08
    bx lr
}
#pragma thumb reset
#else
#error "Bios_RampSoundBias_Arm7: stepping the sound bias register (BIOS call 0x08) has no portable C equivalent"
#endif
