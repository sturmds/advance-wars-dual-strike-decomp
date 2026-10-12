#include "sdk/snd.h"

void SND_StartTimer(u32 channelMask, u32 captureMask, u32 alarmMask,
                    u32 flags) {
  Sound_PostCommandArgs(10, channelMask, captureMask, alarmMask, flags);
}
