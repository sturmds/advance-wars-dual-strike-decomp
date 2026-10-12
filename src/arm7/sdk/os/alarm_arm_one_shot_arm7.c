#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fdc10(void);
void func_arm7_037fd2f4(OSAlarm *alarm, OSTick fire);

/*
 * ARM7 copy of OS_SetAlarm: the handler runs once, delay ticks from now.
 * Stops the system if the alarm is NULL or already set.
 */
void Alarm_ArmOneShot_Arm7(OSAlarm *alarm, OSTick delay, OSAlarmHandler handler, void *arg)
{
    int irqState;

    if (alarm == NULL || alarm->handler != NULL) {
        func_arm7_037fdc10();
    }
    irqState = func_arm7_037fdb80();
    alarm->period = 0;
    alarm->handler = handler;
    alarm->arg = arg;
    func_arm7_037fd2f4(alarm, Tick_Read64_Arm7() + delay);
    func_arm7_037fdb94(irqState);
}
