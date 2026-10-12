#include "types.h"

/* Semihosting call SYS_EXIT (0x18): reports the application exit to the debugger. */
asm void func_021295e8(void)
{
    mov r1, #0
    mov r0, #0x18
    swi 0x123456
    mov pc, lr
}
