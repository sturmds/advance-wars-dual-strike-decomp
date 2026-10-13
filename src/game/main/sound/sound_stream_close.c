#include "types.h"
#include "game/sound_stream.h"

void OS_LockMutex(void* mutex);
void OS_UnlockMutex(void* mutex);
void SoundStrm_StopIfActive(SoundStrm* strm);
void SoundStream_Release(SoundStream* stream);

/* Closes a stream: stops its playback, cancels a file read in progress and
 * releases it, all under the mutex. */
void SoundStream_Close(SoundStream* stream) {
  OS_LockMutex(data_022a8ad8);
  if (stream->flags.playing) {
    SoundStrm_StopIfActive(&stream->strm);
  }
  if (stream->flags.fileOpen) {
    FS_CancelFile(&stream->file);
  }
  SoundStream_Release(stream);
  OS_UnlockMutex(data_022a8ad8);
}
