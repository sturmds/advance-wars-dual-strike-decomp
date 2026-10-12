#include "types.h"
#include "game/sound_strm.h"

void SND_StopTimer(u32 channelMask, int arg1, u32 alarmMask, int arg3);
int SND_GetCurrentCommandTag(void);
void SND_WaitForCommandProc(int arg0);
void SND_FlushCommand(int arg0);
void func_021148a4(SoundSleepHook* hook);
void func_021148bc(SoundSleepHook* hook);
void SoundStrm_Release(SoundStrm* strm);

/* Stops a running stream and drops its sleep hooks, then releases its alarm. */
void SoundStrm_Stop(SoundStrm* strm) {
  int saved;

  if (strm->flags.running) {
    SND_StopTimer(strm->channelMask, 0, 1 << strm->alarm, 0);
    func_021148bc(&strm->preSleep);
    func_021148a4(&strm->postSleep);
    strm->flags.running = 0;
    saved = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(saved);
  }
  SoundStrm_Release(strm);
}
