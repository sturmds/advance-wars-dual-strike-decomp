#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x08 with r0 = 1: raises the sound bias level, one step
   every delay loop iterations. */
#pragma thumb on
asm void Bios_RampSoundBiasUp_Arm7(int delay)
{
    mov r1, r0
    mov r0, #1
    swi 0x08
    bx lr
}
#pragma thumb reset
#else
#error "Bios_RampSoundBiasUp_Arm7: stepping the sound bias register (BIOS call 0x08) has no portable C equivalent"
#endif
