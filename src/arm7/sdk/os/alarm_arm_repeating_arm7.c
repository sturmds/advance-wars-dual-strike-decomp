#include "types.h"
#include "sdk/os_arm7.h"

/*
 * ARM7 copy of OS_SetPeriodicAlarm: the handler runs at start + n * period.
 * Stops the system if the alarm is NULL or already set.
 */
void Alarm_ArmRepeating_Arm7(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg)
{
    int irqState;

    if (alarm == NULL || alarm->handler != NULL) {
        Cpu_HaltForever_Arm7();
    }
    irqState = Cpu_MaskIrq_Arm7();
    alarm->period = period;
    alarm->start = start;
    alarm->handler = handler;
    alarm->arg = arg;
    Alarm_QueueByFireTime_Arm7(alarm, 0);
    Cpu_RestoreIrqBit_Arm7(irqState);
}
