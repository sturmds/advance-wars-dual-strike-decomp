#include "types.h"

/*
 * Hand-written SDK assembly. Copies 32 bytes from src + 32 to dst: the
 * second stmia does not advance dst, so it overwrites the first block.
 * Address taken in func_0210b5b8 and func_0210b5d4; purpose not known.
 */
asm void func_0210cb84(void *src, void *dst)
{
    stmfd sp!, {r4-r8}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmfd sp!, {r4-r8}
    bx lr
}
