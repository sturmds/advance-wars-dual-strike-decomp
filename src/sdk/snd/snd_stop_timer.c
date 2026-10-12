#include "sdk/snd.h"

void SND_StopTimer(u32 channelMask, u32 captureMask, u32 alarmMask, u32 flags) {
  int bit;
  u32 rest = alarmMask;

  for (bit = 0; bit < 8 && rest != 0; bit++, rest >>= 1) {
    if (rest & 1) {
      SoundAlarm_BumpTag(bit);
    }
  }
  Sound_PostCommandArgs(11, channelMask, captureMask, alarmMask, flags);
}
