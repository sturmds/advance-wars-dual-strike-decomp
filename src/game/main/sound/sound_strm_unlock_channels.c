#include "types.h"
#include "game/sound_strm.h"

void Sound_UnlockChannels(u32 channelMask);

/* Gives back the hardware channels the stream locked. */
void SoundStrm_UnlockChannels(SoundStrm* strm) {
  if (strm->channelMask == 0) {
    return;
  }
  Sound_UnlockChannels(strm->channelMask);
  strm->channelMask = 0;
  strm->channelCount = 0;
}
