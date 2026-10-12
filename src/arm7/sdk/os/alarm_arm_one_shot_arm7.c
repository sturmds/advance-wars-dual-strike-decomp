#include "types.h"
#include "sdk/os_arm7.h"

/*
 * ARM7 copy of OS_SetAlarm: the handler runs once, delay ticks from now.
 * Stops the system if the alarm is NULL or already set.
 */
void Alarm_ArmOneShot_Arm7(OSAlarm *alarm, OSTick delay, OSAlarmHandler handler, void *arg)
{
    int irqState;

    if (alarm == NULL || alarm->handler != NULL) {
        Cpu_HaltForever_Arm7();
    }
    irqState = Cpu_MaskIrq_Arm7();
    alarm->period = 0;
    alarm->handler = handler;
    alarm->arg = arg;
    Alarm_QueueByFireTime_Arm7(alarm, Tick_Read64_Arm7() + delay);
    Cpu_RestoreIrqBit_Arm7(irqState);
}
