#include "types.h"
#include "game/sound_stream.h"
#include "sdk/os.h"

void SoundStream_UnlockChannels(SoundStream* stream);
void SoundStream_Detach(SoundStream* stream);
void SoundStreamRequest_Free(SoundStreamRequest* request);

/* Releases the stream: gives back its channels and closes its file, takes its
 * queued refill requests off the thread's list, then clears the slot's
 * flags. Does nothing when the file is not open. */
void SoundStream_Release(SoundStream* stream) {
  int irqState;
  SoundStreamRequest* request;
  SoundStreamRequest* next;

  if (!stream->flags.fileOpen) {
    return;
  }
  SoundStream_UnlockChannels(stream);
  FS_CloseFile(&stream->file);
  irqState = OS_DisableInterrupts();
  for (request = LinkList_GetNext(&data_022a8ab8, NULL); request != NULL;
       request = next) {
    next = LinkList_GetNext(&data_022a8ab8, request);
    if (request->stream == stream) {
      LinkList_Remove(&data_022a8ab8, request);
      SoundStreamRequest_Free(request);
    }
  }
  OS_RestoreInterrupts(irqState);
  SoundStream_Detach(stream);
}
