#include "sdk/snd.h"

u32 func_02110800(u32 a, u32 b, u32 c);

void SND_SetupAlarm(u32 alarm, u32 tick, u32 period, u32 handler, u32 arg)
{
    Sound_PostCommandArgs(16, alarm, tick, period, func_02110800(alarm, handler, arg));
}
