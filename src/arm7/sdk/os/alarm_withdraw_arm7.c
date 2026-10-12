#include "types.h"
#include "sdk/os_arm7.h"

extern AlarmList gOsiAlarmQueue_Arm7;

/* ARM7 copy of OS_CancelAlarm: takes a set alarm out of the queue. */
void Alarm_Withdraw_Arm7(OSAlarm *alarm)
{
    int irqState = Cpu_MaskIrq_Arm7();
    OSAlarm *next;

    if (alarm->handler == NULL) {
        Cpu_RestoreIrqBit_Arm7(irqState);
        return;
    }

    next = alarm->next;
    if (next == NULL) {
        gOsiAlarmQueue_Arm7.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }
    if (alarm->prev != NULL) {
        alarm->prev->next = next;
    } else {
        gOsiAlarmQueue_Arm7.head = next;
        if (next != NULL) {
            /* The first alarm changed: restart timer 1 for it. */
            Alarm_StartTimer1ForDue_Arm7(next);
        }
    }

    alarm->handler = NULL;
    alarm->period = 0;
    Cpu_RestoreIrqBit_Arm7(irqState);
}
