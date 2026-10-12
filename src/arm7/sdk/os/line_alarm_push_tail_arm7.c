#include "types.h"
#include "sdk/os_arm7.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;
void LineAlarm_ProgramVcountIrq_Arm7(OSVAlarm *alarm);

/* Adds the alarm at the end of the V-alarm queue. If the queue was empty, sets the interrupt for it. */
void LineAlarm_PushTail_Arm7(OSVAlarm *alarm)
{
    OSVAlarm *tail = gOsiVAlarmQueue_Arm7.tail;

    alarm->prev = tail;
    alarm->next = NULL;
    gOsiVAlarmQueue_Arm7.tail = alarm;
    if (tail != NULL) {
        tail->next = alarm;
    } else {
        gOsiVAlarmQueue_Arm7.head = alarm;
        LineAlarm_ProgramVcountIrq_Arm7(alarm);
    }
}
