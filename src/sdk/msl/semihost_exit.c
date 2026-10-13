#include "types.h"

/* Semihosting call SYS_EXIT (0x18): reports the application exit to the debugger. */
#ifdef __MWERKS__
asm void Semihost_Exit(void)
{
    mov r1, #0
    mov r0, #0x18
    swi 0x123456
    mov pc, lr
}
#else
#error "Semihost_Exit: the swi 0x123456 debugger trap has no portable C equivalent"
#endif
