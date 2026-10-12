#include "types.h"

/*
 * Finds out whether main RAM is 4 MB (retail) or 8 MB (debug hardware) and
 * stores 1 or 2 in the shared halfword at 0x027ffffa. With 4 MB, main RAM
 * repeats every 4 MB, so 0x023ffffa reads back what was written to
 * 0x027ffffa; the test writes 0 and 1 and compares both.
 */
asm void DetectMainMemorySize(void)
{
    mov r0, #1
    mov r1, #0
    ldr r2, =0x027ffffa
    sub r3, r2, #0x400000
loop:
    strh r1, [r2]
    ldrh r12, [r3]
    cmp r1, r12
    movne r0, #2
    bne done
    add r1, r1, #1
    cmp r1, #2
    bne loop
done:
    strh r0, [r2]
    bx lr
}
