#include "types.h"

extern u32 BuildInfo_Arm7[];    /* module parameters the linker fills in */

/*
 * Copies the ARM7 autoload blocks (main RAM and WRAM parts) to where they
 * run. Same loop as the ARM9 do_autoload: BuildInfo_Arm7[0] and [1] bound
 * a list of three-word entries (destination, size to copy, size to clear
 * after it), and the data for all blocks follows one another from
 * BuildInfo_Arm7[2]. Ends by branching to an empty callback that sits after
 * the literal word, inside this function.
 */
asm void do_autoload_Arm7(void)
{
    /* load build_info below; the assembler takes no pc-relative label
       here, and `=BuildInfo_Arm7` would put the word after the callback */
    ldr r0, [pc, #0x4c]
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
    b callback
build_info:
    dcd BuildInfo_Arm7
callback:
    bx lr
}
