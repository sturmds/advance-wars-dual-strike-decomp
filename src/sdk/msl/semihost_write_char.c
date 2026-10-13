#include "types.h"

/* Semihosting call SYS_WRITEC (3): writes the character at *charPtr to the debugger console. */
#ifdef __MWERKS__
asm void Semihost_WriteChar(const char *charPtr)
{
    str lr, [sp, #-4]!
    mov r1, r0
    mov r0, #3
    swi 0x123456
    ldr pc, [sp], #4
}
#else
#error "Semihost_WriteChar: the swi 0x123456 debugger trap has no portable C equivalent"
#endif
