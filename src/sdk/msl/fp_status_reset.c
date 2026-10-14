#include "sdk/fp_status.h"
#include "sdk/msl.h"

/*
 * Called from Entry. Clears the word __fp_status returns and the handlers
 * 1-5 of the table FpTrap_GetHandlers returns. Written as assembly
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
    bl FpTrap_GetHandlers
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
    FpTrapHandler *handlers;
    int i;

    *__fp_status() = 0;
    handlers = FpTrap_GetHandlers();
    for (i = 1; i < FP_TRAP_HANDLER_COUNT; i++) {
        handlers[i] = NULL;
    }
}
#endif
