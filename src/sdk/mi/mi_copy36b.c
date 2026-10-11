#include "sdk/mi.h"

/* Hand-written ARM assembly in the SDK: copies nine words with three load-multiples. */
asm void MI_Copy36B(const void *src, void *dest)
{
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    bx lr
}
