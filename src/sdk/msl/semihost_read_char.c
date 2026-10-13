#include "types.h"

/* Semihosting call SYS_READC (7): reads one character from the debugger console. */
#ifdef __MWERKS__
asm int Semihost_ReadChar(void)
{
    str lr, [sp, #-4]!
    mov r1, #0
    mov r0, #7
    swi 0x123456
    ldr pc, [sp], #4
}
#else
#error "Semihost_ReadChar: the swi 0x123456 debugger trap has no portable C equivalent"
#endif
