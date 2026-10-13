#include "types.h"
#include "game/sound.h"

void SND_LockChannel(u32 channelMask, int flags);

/* Takes the channels in channelMask away from the sound processor. Returns
 * FALSE, locking nothing, if any of them is locked already. */
BOOL Sound_LockChannels(u32 channelMask) {
  if (channelMask == 0) {
    return TRUE;
  }
  if (channelMask & gLockedSoundChannels) {
    return FALSE;
  }
  SND_LockChannel(channelMask, 0);
  gLockedSoundChannels |= channelMask;
  return TRUE;
}
