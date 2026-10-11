#include "types.h"
#include "game/sound_strm.h"

void SND_StartTimer(u32 channelMask, int arg1, u32 alarmMask, int arg3);
void func_021148d4(SoundSleepHook *hook);
void func_021148ec(SoundSleepHook *hook);

/* Starts the channels and, the first time, registers the sleep and wake-up hooks. */
void SoundStrm_Start(SoundStrm *strm)
{
    SND_StartTimer(strm->channelMask, 0, 1 << strm->alarm, 0);
    if (!strm->flags.running) {
        func_021148ec(&strm->preSleep);
        func_021148d4(&strm->postSleep);
        strm->flags.running = 1;
    }
}
