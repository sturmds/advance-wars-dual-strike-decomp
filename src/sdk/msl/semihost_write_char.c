#include "types.h"

/* Semihosting call SYS_WRITEC (3): writes the character at *charPtr to the debugger console. */
asm void Semihost_WriteChar(const char *charPtr)
{
    str lr, [sp, #-4]!
    mov r1, r0
    mov r0, #3
    swi 0x123456
    ldr pc, [sp], #4
}
