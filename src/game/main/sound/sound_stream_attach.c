#include "types.h"
#include "game/sound_stream.h"

void func_02108f2c(void **handle);
void SoundStream_Close(SoundStream *stream);

/* Claims stream slot slotNo for handle. The handle lets go of the stream it
 * had. Returns NULL when the slot has no block memory, or when it is in use
 * with a higher priority than priority; otherwise closes what the slot was
 * playing, ties it to handle and returns it. */
SoundStream *SoundStream_Attach(void **handle, int slotNo, int priority)
{
    SoundStream *stream;

    if (*handle != NULL) {
        func_02108f2c(handle);
    }
    stream = &data_022a9300[slotNo];
    if (stream->buffer == NULL) {
        return NULL;
    }
    if (stream->flags.fileOpen) {
        if (priority < (int)stream->unk_14c) {
            return NULL;
        }
        SoundStream_Close(stream);
    }
    stream->unk_14c = priority;
    stream->flags.fileOpen = 1;
    stream->handle = handle;
    *handle = stream;
    return stream;
}
