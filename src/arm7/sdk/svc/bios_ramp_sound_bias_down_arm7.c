#include "sdk/svc.h"

/* ARM7 BIOS call 0x08 with r0 = 0: lowers the sound bias level, one step
   every delay loop iterations. */
#pragma thumb on
asm void Bios_RampSoundBiasDown_Arm7(int delay)
{
    mov r1, r0
    mov r0, #0
    swi 0x08
    bx lr
}
#pragma thumb reset
