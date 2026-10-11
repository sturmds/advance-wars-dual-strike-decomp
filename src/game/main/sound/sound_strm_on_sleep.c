#include "types.h"
#include "game/sound_strm.h"

void SND_StopTimer(u32 channelMask, int arg1, u32 alarmMask, int arg3);
int SND_GetCurrentCommandTag(void);
void SND_WaitForCommandProc(int arg0);
void SND_FlushCommand(int arg0);

/* Sleep hook: stops the channels and the alarm of a running stream. */
void SoundStrm_OnSleep(SoundStrm *strm)
{
    int saved;

    if (!strm->flags.running) {
        return;
    }
    SND_StopTimer(strm->channelMask, 0, 1 << strm->alarm, 0);
    saved = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(saved);
}
