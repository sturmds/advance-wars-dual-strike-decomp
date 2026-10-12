#include "types.h"

void func_arm7_037fd074(void);

/*
 * Timer 1 interrupt handler of the alarm system. Kept as asm: from C the
 * compiler turns the call into a tail jump, while the original calls and
 * returns without the usual 8-byte stack alignment.
 */
asm void Alarm_OnTimer1Irq_Arm7(void *arg)
{
    stmfd sp!, {lr}
    bl func_arm7_037fd074
    ldmfd sp!, {lr}
    bx lr
}
