#include "types.h"
#include "game/sound_stream.h"

void SoundStreamHandle_Detach(void** handle);
void SoundStream_Close(SoundStream* stream);

/* Claims stream slot slotNo for handle. The handle lets go of the stream it
 * had. Returns NULL when the slot has no block memory, or when it is in use
 * with a higher priority than priority; otherwise closes what the slot was
 * playing, ties it to handle and returns it. */
SoundStream* SoundStream_Attach(void** handle, int slotNo, int priority) {
  SoundStream* stream;

  if (*handle != NULL) {
    SoundStreamHandle_Detach(handle);
  }
  stream = &data_022a9300[slotNo];
  if (stream->buffer == NULL) {
    return NULL;
  }
  if (stream->flags.fileOpen) {
    if (priority < stream->priority) {
      return NULL;
    }
    SoundStream_Close(stream);
  }
  stream->priority = priority;
  stream->flags.fileOpen = 1;
  stream->handle = handle;
  *handle = stream;
  return stream;
}
