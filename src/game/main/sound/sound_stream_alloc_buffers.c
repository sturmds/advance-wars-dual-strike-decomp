#include "types.h"
#include "game/sound_heap.h"
#include "game/sound_stream.h"

const u8* func_02106420(int strmPlayerNo);
void* SoundHeap_AllocItem(SoundHeap* heap, u32 size,
                          void (*onUnload)(void*, u32, u32, u32), u32 arg3,
                          u32 arg4);
void SoundStream_Close(SoundStream* stream);
void SoundStream_ReleaseBuffer(void* buffer, u32 unused, SoundStream* stream);

/* Gives each stream slot its block memory on heap: the hardware channel
 * numbers the slot may use come from the archive's stream player table, and
 * the buffer is 0x800 bytes per channel. A slot whose table entry is missing
 * is skipped. Returns FALSE when the heap runs out. */
BOOL SoundStream_AllocBuffers(SoundHeap* heap) {
  int i;
  int j;
  u32 size;
  void* buffer;
  const u8* info;
  SoundStream* stream = data_022a9300;

  for (i = 0; i < SOUND_STREAM_COUNT; i++, stream++) {
    info = func_02106420(i);
    if (info == NULL) {
      continue;
    }
    stream->slotChannelCount = info[0];
    for (j = 0; j < info[0]; j++) {
      stream->slotChannels[j] = (&info[j])[1];
    }
    if (heap == NULL) {
      continue;
    }
    size = stream->slotChannelCount << 11;
    buffer = SoundHeap_AllocItem(heap, size, (void*)SoundStream_ReleaseBuffer,
                                 (u32)stream, 0);
    if (buffer == NULL) {
      return FALSE;
    }
    SoundStream_Close(stream);
    stream->buffer = buffer;
    stream->bufferSize = size;
  }
  return TRUE;
}
