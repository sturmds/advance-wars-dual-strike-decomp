#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fdc10(void);
void func_arm7_037fd2f4(OSAlarm *alarm, OSTick fire);

/*
 * ARM7 copy of OS_SetPeriodicAlarm: the handler runs at start + n * period.
 * Stops the system if the alarm is NULL or already set.
 */
void Alarm_ArmRepeating_Arm7(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg)
{
    int irqState;

    if (alarm == NULL || alarm->handler != NULL) {
        func_arm7_037fdc10();
    }
    irqState = func_arm7_037fdb80();
    alarm->period = period;
    alarm->start = start;
    alarm->handler = handler;
    alarm->arg = arg;
    func_arm7_037fd2f4(alarm, 0);
    func_arm7_037fdb94(irqState);
}
