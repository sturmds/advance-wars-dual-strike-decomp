#include "types.h"

extern void Exception_FillContextRecord(void);
extern void Exception_RunUserHook(void);

#ifdef __MWERKS__
/*
 * Called from the exception handler (OSi_ExceptionHandler). Written as assembly
 * because the original keeps the stack 4 bytes off 8-byte alignment, which
 * compiled C never does.
 */
asm void func_0210e6f8(void)
{
    stmdb sp!, {lr}
    bl Exception_FillContextRecord
    bl Exception_RunUserHook
    ldmia sp!, {lr}
    bx lr
}
#else
#error "func_0210e6f8: passing r0 and ip through unchanged to Exception_FillContextRecord has no portable C equivalent"
#endif
