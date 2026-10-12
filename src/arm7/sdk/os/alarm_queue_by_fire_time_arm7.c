#include "types.h"
#include "sdk/os_arm7.h"

extern AlarmList gOsiAlarmQueue_Arm7;
void func_arm7_037fd494(OSAlarm *alarm);
u64 func_arm7_03806b44(u64 dividend, u64 divisor);

/*
 * Puts an alarm into the queue, sorted by fire time. A periodic alarm
 * ignores fire and takes the first start + n * period that has not passed.
 */
void Alarm_QueueByFireTime_Arm7(OSAlarm *alarm, OSTick fire)
{
    OSAlarm *queued;
    OSAlarm *prev;

    if (alarm->period != 0) {
        OSTick now = Tick_Read64_Arm7();

        fire = alarm->start;
        if (fire < now) {
            OSTick period = alarm->period;

            /* The compiler's 64-bit unsigned divide, called by name in the original. */
            fire += period * (func_arm7_03806b44(now - fire, period) + 1);
        }
    }
    alarm->fire = fire;

    for (queued = gOsiAlarmQueue_Arm7.head; queued != NULL; queued = queued->next) {
        if ((s64)(fire - queued->fire) < 0) {
            alarm->prev = queued->prev;
            queued->prev = alarm;
            alarm->next = queued;
            if (alarm->prev != NULL) {
                alarm->prev->next = alarm;
            } else {
                gOsiAlarmQueue_Arm7.head = alarm;
                func_arm7_037fd494(alarm);
            }
            return;
        }
    }

    alarm->next = NULL;
    prev = gOsiAlarmQueue_Arm7.tail;
    gOsiAlarmQueue_Arm7.tail = alarm;
    alarm->prev = prev;
    if (prev != NULL) {
        prev->next = alarm;
    } else {
        gOsiAlarmQueue_Arm7.tail = alarm;
        gOsiAlarmQueue_Arm7.head = alarm;
        func_arm7_037fd494(alarm);
    }
}
