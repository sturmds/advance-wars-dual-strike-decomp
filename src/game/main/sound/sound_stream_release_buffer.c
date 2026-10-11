#include "types.h"
#include "game/sound_stream.h"

extern u8 data_022a8ad8[];             /* mutex around the stream slots and request lists */

void func_0210e034(void *mutex);
void func_0210dfc4(void *mutex);
void SoundStream_Close(SoundStream *stream);
void SoundStrm_UnlockChannels(SoundStrm *strm);

/* Run when the sound heap drops the block memory of a stream slot: closes
 * the stream and empties the slot. Does nothing when buffer is not the
 * slot's. */
void SoundStream_ReleaseBuffer(void *buffer, u32 unused, SoundStream *stream)
{
    if (buffer != stream->buffer) {
        return;
    }
    func_0210e034(data_022a8ad8);
    SoundStream_Close(stream);
    stream->buffer = NULL;
    stream->bufferSize = 0;
    stream->slotChannelCount = 0;
    if (stream->channelLockCount > 0) {
        SoundStrm_UnlockChannels(&stream->strm);
        stream->channelLockCount = 0;
    }
    func_0210dfc4(data_022a8ad8);
}
