#include "sdk/snd_seq.h"
#include "sdk/snd.h"

/* Starts a PCM sample on the channel; any channel can play one. */
BOOL SndExChannel_StartPcm(SndExChannel* channel, const SndWaveParam* wave,
                           const void* data, int length) {
  channel->type = VOICE_TYPE_SAMPLED;
  channel->wave = *wave;
  channel->data = data;
  SndExChannel_Start(channel, length);
  return TRUE;
}
