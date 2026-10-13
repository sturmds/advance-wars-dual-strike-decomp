#include "types.h"
#include "game/sound.h"

void SND_UnlockChannel(u32 channelMask, int flags);

/* Gives the channels in channelMask back to the sound processor. */
void Sound_UnlockChannels(u32 channelMask) {
  if (channelMask == 0) {
    return;
  }
  SND_UnlockChannel(channelMask, 0);
  gLockedSoundChannels &= ~channelMask;
}
