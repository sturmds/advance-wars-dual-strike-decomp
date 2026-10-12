#include "sdk/svc.h"

/* ARM7 BIOS call 0x04: halt until a selected interrupt occurs. Sets r2 to 0
   and ip to the I/O register base (0x04000000) before the call. The
   constant is written as instructions after the return: the assembler pads
   a literal pool with a nop, the original with zeros. */
#pragma thumb on
asm void Bios_AwaitIrqFlags_Arm7(int clearFlags, u32 intrFlags)
{
    ldr r2, [pc, #8]
    mov ip, r2
    mov r2, #0
    swi 0x04
    bx lr
    lsl r0, r0, #0
    lsl r0, r0, #0
    lsl r0, r0, #16
}
#pragma thumb reset
