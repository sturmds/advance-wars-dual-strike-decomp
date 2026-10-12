#include "sdk/snd.h"

void Alarm_Withdraw_Arm7(void *alarm);

void SndArm7_SetupAlarm(int alarmNo, u64 start, u64 period, int tag)
{
    SndArm7Alarm *alarm = &data_arm7_03809010[alarmNo];

    if (alarm->active != 0) {
        Alarm_Withdraw_Arm7(&alarm->osAlarm);
        alarm->active = 0;
    }
    alarm->start = start;
    alarm->period = period;
    alarm->tag = tag;
}
