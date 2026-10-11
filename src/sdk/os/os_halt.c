#include "types.h"

/* Stops the CPU until the next interrupt (CP15 wait for interrupt). */
asm void OS_Halt(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c0, 4
    bx lr
}
