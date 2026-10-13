#include "types.h"

/*
 * Restores the registers saved in buf and continues after the setjmp that
 * filled it, with value as its result (1 if value is 0).
 */
#ifdef __MWERKS__
asm void longjmp(void *buf, int value)
{
    ldmia r0, {r4-r11, sp, lr}
    movs r0, r1
    moveq r0, #1
    bx lr
}
#else
#error "longjmp: restoring r4-r11, sp and lr by hand has no portable C equivalent"
#endif
