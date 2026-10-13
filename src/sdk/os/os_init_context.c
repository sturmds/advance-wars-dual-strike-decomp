#include "types.h"
#include "sdk/os.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly in the original. Sets up the registers for a new
 * thread: pc + 4 in pcPlus4, sp - 0x40 as the stack, the given sp
 * for SVC mode, System mode (Thumb when bit 0 of pc is set), and every other
 * register zero.
 */
asm void OS_InitContext(OSContext *context, u32 pc, u32 sp)
{
    add r1, r1, #4
    str r1, [r0, #0x40]     /* pcPlus4 */
    str r2, [r0, #0x44]     /* spSvc */
    sub r2, r2, #0x40
    str r2, [r0, #0x38]     /* sp */
    ands r1, r1, #1
    movne r1, #0x3f
    moveq r1, #0x1f
    str r1, [r0]            /* cpsr */
    mov r1, #0
    str r1, [r0, #0x4]
    str r1, [r0, #0x8]
    str r1, [r0, #0xc]
    str r1, [r0, #0x10]
    str r1, [r0, #0x14]
    str r1, [r0, #0x18]
    str r1, [r0, #0x1c]
    str r1, [r0, #0x20]
    str r1, [r0, #0x24]
    str r1, [r0, #0x28]
    str r1, [r0, #0x2c]
    str r1, [r0, #0x30]
    str r1, [r0, #0x34]
    str r1, [r0, #0x3c]     /* lr */
    bx lr
}
#else
/* The context's cpContext is left as it was, as in the assembly. */
void OS_InitContext(OSContext *context, u32 pc, u32 sp) {
  int i;

  context->pcPlus4 = pc + 4;
  context->spSvc = sp;
  context->sp = sp - 0x40;
  context->cpsr = (pc & 1) ? 0x3f : 0x1f;
  for (i = 0; i < 13; i++) {
    context->r[i] = 0;
  }
  context->lr = 0;
}
#endif
