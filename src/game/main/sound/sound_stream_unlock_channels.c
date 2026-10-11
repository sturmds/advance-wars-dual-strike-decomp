#include "types.h"
#include "game/sound_stream.h"

void SoundStrm_UnlockChannels(SoundStrm *strm);

/* Drops one use of the stream's hardware channels; the last one gives them
 * back. */
void SoundStream_UnlockChannels(SoundStream *stream)
{
    if (stream->channelLockCount == 0) {
        return;
    }
    stream->channelLockCount--;
    if (stream->channelLockCount != 0) {
        return;
    }
    SoundStrm_UnlockChannels(&stream->strm);
}
