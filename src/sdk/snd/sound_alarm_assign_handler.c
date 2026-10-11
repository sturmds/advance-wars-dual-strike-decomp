#include "sdk/snd.h"

extern SndAlarm data_022ab740[SND_ALARM_COUNT];

u32 SoundAlarm_AssignHandler(u32 alarmNo, SndAlarmHandler handler, u32 arg)
{
    SndAlarm *alarm = &data_022ab740[alarmNo];

    alarm->handler = handler;
    alarm->arg = arg;
    alarm->tag++;
    return alarm->tag;
}
