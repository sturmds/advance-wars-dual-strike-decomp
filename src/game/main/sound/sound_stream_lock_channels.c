#include "types.h"
#include "game/sound_stream.h"

BOOL SoundStrm_LockChannels(SoundStrm* strm, int count, const u8* channels);

/* Adds one use of the stream's hardware channels; the first locks the first count
 * channels of the list. Returns FALSE when they cannot be locked. */
BOOL SoundStream_LockChannels(SoundStream* stream, int count,
                              const u8* channels) {
  if (stream->channelLockCount == 0) {
    if (!SoundStrm_LockChannels(&stream->strm, count, channels)) {
      return FALSE;
    }
  }
  stream->channelLockCount++;
  return TRUE;
}
