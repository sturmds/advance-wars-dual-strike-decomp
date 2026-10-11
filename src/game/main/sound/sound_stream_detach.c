#include "types.h"
#include "game/sound_stream.h"

/* Cuts the stream loose from the caller's handle and marks the slot as not
 * in use and not playing. */
void SoundStream_Detach(SoundStream *stream)
{
    if (stream->handle != NULL) {
        *stream->handle = NULL;
        stream->handle = NULL;
    }
    stream->flags.fileOpen = 0;
    stream->flags.startPending = 0;
    stream->flags.playing = 0;
}
