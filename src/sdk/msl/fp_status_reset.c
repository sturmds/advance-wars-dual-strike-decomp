#include "sdk/fp_status.h"

extern u8 *func_02129754(void);

/*
 * Called from Entry. Clears the word __fp_status returns and the five words
 * after the first in the block func_02129754 returns. Written as assembly
 * because the original keeps the stack 4 bytes off 8-byte alignment, which
 * compiled C never does.
 */
#ifdef __MWERKS__
asm void FpStatus_Reset(void)
{
    stmfd sp!, {lr}
    bl __fp_status
    mov r1, #0
    str r1, [r0]
    bl func_02129754
    mov r1, #0
    str r1, [r0, #4]
    str r1, [r0, #8]
    str r1, [r0, #12]
    str r1, [r0, #16]
    str r1, [r0, #20]
    ldmfd sp!, {lr}
    bx lr
}
#else
void FpStatus_Reset(void)
{
    u32 *words;

    *__fp_status() = 0;
    words = (u32 *)func_02129754();
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
    words[4] = 0;
    words[5] = 0;
}
#endif
