#include "sdk/snd.h"

void func_arm7_037fd174(void *alarm);

void SndArm7_SetupAlarm(int alarmNo, u64 start, u64 period, int tag)
{
    SndArm7Alarm *alarm = &data_arm7_03809010[alarmNo];

    if (alarm->active != 0) {
        func_arm7_037fd174(alarm->osAlarm);
        alarm->active = 0;
    }
    alarm->start = start;
    alarm->period = period;
    alarm->tag = tag;
}
