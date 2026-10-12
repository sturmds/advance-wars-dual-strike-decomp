#include "sdk/svc.h"

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
