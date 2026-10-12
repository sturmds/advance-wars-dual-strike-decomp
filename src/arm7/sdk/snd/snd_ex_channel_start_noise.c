#include "sdk/snd_seq.h"
#include "sdk/snd.h"

/* Starts white noise on the channel; only hardware channels 14 and 15 can play noise. */
BOOL SndExChannel_StartNoise(SndExChannel* channel, int length) {
  if (channel->id < 14) {
    return FALSE;
  }
  if (channel->id > 15) {
    return FALSE;
  }
  channel->type = VOICE_TYPE_NOISE;
  channel->wave.timer = VOICE_SQUARE_KEY60_RELOAD;
  SndExChannel_Start(channel, length);
  return TRUE;
}
