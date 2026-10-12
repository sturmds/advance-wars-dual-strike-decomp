#include "types.h"
#include "sdk/os_arm7.h"

extern AlarmList gOsiAlarmQueue_Arm7;
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void Alarm_StartTimer1ForDue_Arm7(OSAlarm *alarm);

/* ARM7 copy of OS_CancelAlarm: takes a set alarm out of the queue. */
void Alarm_Withdraw_Arm7(OSAlarm *alarm)
{
    int irqState = func_arm7_037fdb80();
    OSAlarm *next;

    if (alarm->handler == NULL) {
        func_arm7_037fdb94(irqState);
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
    func_arm7_037fdb94(irqState);
}
