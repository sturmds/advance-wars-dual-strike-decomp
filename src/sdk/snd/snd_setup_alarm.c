#include "sdk/snd.h"

void SND_SetupAlarm(u32 alarm, u32 tick, u32 period, SndAlarmHandler handler, u32 arg)
{
    Sound_PostCommandArgs(16, alarm, tick, period, SoundAlarm_AssignHandler(alarm, handler, arg));
}
