#include "types.h"

extern u32 BuildInfo[];     /* module parameters the linker fills in */
void AutoloadCallback(int arg);

/*
 * Copies the autoload blocks (ITCM, DTCM and the like) to where they run.
 * BuildInfo[0] and [1] bound a list of three-word entries: destination,
 * size to copy, size to clear after it. The data for all blocks follows one
 * another from BuildInfo[2]. Ends by jumping to AutoloadCallback.
 */
asm void do_autoload(void)
{
    ldr r0, =BuildInfo
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
next_block:
    cmp r1, r2
    beq done
    ldr r4, [r1], #4
    ldr r5, [r1], #4
    add r6, r4, r5
copy:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi copy
    ldr r5, [r1], #4
    add r6, r4, r5
    mov r7, #0
clear:
    cmp r4, r6
    strcc r7, [r4], #4
    bcc clear
    beq next_block
done:
    b AutoloadCallback
}
