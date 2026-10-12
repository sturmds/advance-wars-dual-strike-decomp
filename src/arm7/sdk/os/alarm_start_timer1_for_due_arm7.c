#include "types.h"
#include "sdk/os_arm7.h"

void func_arm7_037fb6fc(int timer, void (*handler)(void *), void *arg);
void func_arm7_037fb690(u32 mask);
void Alarm_OnTimer1Irq_Arm7(void *arg);

/*
 * Starts timer 1 so that it overflows when the alarm is due. Timer 1 counts
 * up from the loaded value at the same rate as the tick counter. An alarm
 * already due fires after two counts; one 0x10000 or more counts away waits
 * a full timer period and is checked again then.
 */
void Alarm_StartTimer1ForDue_Arm7(OSAlarm *alarm)
{
    OSTick now;
    s64 delay;
    u16 count;

    now = Tick_Read64_Arm7();
    REG_TM1CNT_H = 0;
    delay = alarm->fire - now;
    func_arm7_037fb6fc(1, Alarm_OnTimer1Irq_Arm7, NULL);
    if (delay < 0) {
        count = 0xfffe;
    } else if (delay < 0x10000) {
        count = (u16)~delay;
    } else {
        count = 0;
    }
    REG_TM1CNT_L = count;
    REG_TM1CNT_H = TMCNT_START_IRQ_DIV64;
    func_arm7_037fb690(IRQ_MASK_TIMER1);
}
