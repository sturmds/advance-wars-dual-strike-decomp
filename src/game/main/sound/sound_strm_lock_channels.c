#include "types.h"
#include "game/sound_strm.h"

BOOL Sound_LockChannels(u32 channelMask);

/* Locks the given hardware channels for the stream. Returns FALSE
 * when one of them is locked already. */
BOOL SoundStrm_LockChannels(SoundStrm* strm, int count, const u8* channels) {
  u32 mask = 0;
  int i;

  for (i = 0; i < count; i++) {
    strm->channels[i] = channels[i];
    mask |= 1 << channels[i];
  }
  if (!Sound_LockChannels(mask)) {
    return FALSE;
  }
  strm->channelCount = count;
  strm->channelMask = mask;
  return TRUE;
}
