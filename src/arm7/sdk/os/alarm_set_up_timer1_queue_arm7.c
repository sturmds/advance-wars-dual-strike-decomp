#include "types.h"
#include "sdk/os_arm7.h"

extern u16 data_arm7_03807a8c;
extern AlarmList gOsiAlarmQueue_Arm7;

/*
 * ARM7 copy of OS_InitAlarm. Once only: reserves timer 1, empties the alarm
 * queue and masks the timer 1 interrupt until an alarm is set.
 */
void Alarm_SetUpTimer1Queue_Arm7(void)
{
    if (data_arm7_03807a8c != 0) {
        return;
    }
    data_arm7_03807a8c = 1;
    Timer_MarkInUse_Arm7(1);
    gOsiAlarmQueue_Arm7.head = NULL;
    gOsiAlarmQueue_Arm7.tail = NULL;
    Irq_ClearEnableBits_Arm7(IRQ_MASK_TIMER1);
}
