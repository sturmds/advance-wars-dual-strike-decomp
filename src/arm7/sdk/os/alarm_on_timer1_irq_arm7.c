#include "types.h"
#include "sdk/os_arm7.h"

#ifdef __MWERKS__
/*
 * Timer 1 interrupt handler of the alarm system. Kept as asm: from C the
 * compiler turns the call into a tail jump, while the original calls and
 * returns without the usual 8-byte stack alignment.
 */
asm void Alarm_OnTimer1Irq_Arm7(void *arg)
{
    stmfd sp!, {lr}
    bl Alarm_FireDueAndRearm_Arm7
    ldmfd sp!, {lr}
    bx lr
}
#else
void Alarm_OnTimer1Irq_Arm7(void* arg) { Alarm_FireDueAndRearm_Arm7(); }
#endif
