#include "types.h"
#include "sdk/os_arm7.h"

/*
 * Takes a V-count alarm out of the queue if it is set. Marks it canceled
 * either way, so a periodic alarm whose handler is running is not set again.
 */
void LineAlarm_Withdraw_Arm7(OSVAlarm *alarm)
{
    int irqState = Cpu_MaskIrq_Arm7();

    alarm->canceled = TRUE;
    if (alarm->handler == NULL) {
        Cpu_RestoreIrqBit_Arm7(irqState);
        return;
    }
    LineAlarm_Unlink_Arm7(alarm);
    alarm->handler = NULL;
    Cpu_RestoreIrqBit_Arm7(irqState);
}
