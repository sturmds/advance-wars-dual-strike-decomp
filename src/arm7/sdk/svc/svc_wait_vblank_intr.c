#include "sdk/svc.h"

/* ARM7 BIOS call 0x05: halts until the next V-blank interrupt. Sets r2 to 0
   before the call. */
#pragma thumb on
asm void Bios_SleepUntilVBlank_Arm7(void)
{
    mov r2, #0
    swi 0x05
    bx lr
}
#pragma thumb reset
