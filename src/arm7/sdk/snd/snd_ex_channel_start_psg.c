#include "sdk/snd_seq.h"
#include "sdk/snd.h"

/* Starts a square wave of the given duty on the channel; only hardware channels 8 to 13 can play one. */
BOOL SndExChannel_StartPsg(SndExChannel* channel, int duty, int length) {
  if (channel->id < 8) {
    return FALSE;
  }
  if (channel->id > 13) {
    return FALSE;
  }
  channel->type = VOICE_TYPE_SQUARE_WAVE;
  channel->data = (const void*)duty;
  channel->wave.timer = VOICE_SQUARE_KEY60_RELOAD;
  SndExChannel_Start(channel, length);
  return TRUE;
}
