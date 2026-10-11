#include "types.h"

extern void Exception_FillContextRecord(void);
extern void func_0210e79c(void);

/*
 * Called from the exception handler (OSi_ExceptionHandler). Written as assembly
 * because the original keeps the stack 4 bytes off 8-byte alignment, which
 * compiled C never does.
 */
asm void func_0210e6f8(void)
{
    stmdb sp!, {lr}
    bl Exception_FillContextRecord
    bl func_0210e79c
    ldmia sp!, {lr}
    bx lr
}
