#include "sdk/snd.h"

u64 Tick_Read64_Arm7(void);
void Alarm_Withdraw_Arm7(void *alarm);
void Alarm_ArmRepeating_Arm7(void *alarm, u64 start, u64 period, void (*handler)(int), int arg);
void Alarm_ArmOneShot_Arm7(void *alarm, u64 start, void (*handler)(int), int arg);
void func_arm7_037fd424(void *alarm);
void SndArm7_AlarmHandler(int arg);

void SndArm7_StartAlarm(int alarmNo)
{
    /* needed to match: the declaration order sets the register order */
    u64 start;
    u64 period;
    SndArm7Alarm *alarm;
    int arg;
    alarm = &data_arm7_03809010[alarmNo];
    if (alarm->active != 0) {
        Alarm_Withdraw_Arm7(alarm->osAlarm);
        alarm->active = 0;
    }
    start = alarm->start;
    period = alarm->period;
    arg = alarmNo | (alarm->tag << 8);
    func_arm7_037fd424(alarm->osAlarm);
    if (period == 0) {
        Alarm_ArmOneShot_Arm7(alarm->osAlarm, start, SndArm7_AlarmHandler, arg);
    } else {
        Alarm_ArmRepeating_Arm7(alarm->osAlarm, start + Tick_Read64_Arm7(), period, SndArm7_AlarmHandler, arg);
    }
    alarm->active = 1;
}
