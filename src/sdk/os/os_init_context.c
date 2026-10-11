#include "types.h"
#include "sdk/os.h"

/*
 * Sets up the registers for a new thread. A pc with bit 0 set starts the
 * thread in Thumb state; the mode is System. Not matching yet: the original
 * adds 4 to pc and takes 0x40 off sp in the argument registers (80%).
 * The stores are written out because a loop over r[] compiles differently.
 */
void OS_InitContext(OSContext *context, u32 pc, u32 sp)
{
    u32 pcPlus4 = pc + 4;
    u32 stackTop = sp - 0x40;

    context->pcPlus4 = pcPlus4;
    context->spSvc = sp;
    context->sp = stackTop;
    context->cpsr = (pcPlus4 & 1) ? 0x3f : 0x1f;
    context->r[0] = 0;
    context->r[1] = 0;
    context->r[2] = 0;
    context->r[3] = 0;
    context->r[4] = 0;
    context->r[5] = 0;
    context->r[6] = 0;
    context->r[7] = 0;
    context->r[8] = 0;
    context->r[9] = 0;
    context->r[10] = 0;
    context->r[11] = 0;
    context->r[12] = 0;
    context->lr = 0;
}
