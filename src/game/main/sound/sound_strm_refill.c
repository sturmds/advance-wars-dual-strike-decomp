#include "types.h"
#include "game/sound_strm.h"

extern SoundStrmChannel data_022a8978
    [SOUND_STRM_CHANNEL_COUNT]; /* ring buffer of each hardware channel */
extern void* data_022a8938[SOUND_STRM_CHANNEL_COUNT];

/* Calls the stream callback for the next block, with the address of that
 * block in the ring of each channel, then moves on to the following block. */
void SoundStrm_Refill(SoundStrm* strm, int status) {
  u32 blockSize = strm->bufferSize / strm->blockCount;
  u32 offset = blockSize * strm->blockIndex;
  int i;

  for (i = 0; i < strm->channelCount; i++) {
    u32 buffer = data_022a8978[strm->channels[i]].buffer;

    data_022a8938[i] = (void*)(buffer + offset);
  }
  strm->callback(status, strm->channelCount, data_022a8938, blockSize,
                 strm->format, strm->callbackArg);
  strm->blockIndex++;
  if (strm->blockIndex >= strm->blockCount) {
    strm->blockIndex = 0;
  }
}
