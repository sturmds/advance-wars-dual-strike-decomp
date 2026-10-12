#include "sdk/snd_seq.h"
#include "sdk/snd.h"

/* Returns the channel's LFO offset scaled for its target (pitch, volume or pan), and advances the LFO when update is set. */
int SndExChannel_UpdateLfo(SndExChannel* channel, BOOL update) {
  s64 value = SndLfo_GetValue(&channel->lfo);

  if (value != 0) {
    switch (channel->lfo.param.target) {
      case SND_LFO_VOLUME:
        value *= 60;
        break;
      case SND_LFO_PAN:
        value *= 64;
        break;
      case SND_LFO_PITCH:
        value *= 64;
        break;
    }
    value >>= 14;
  }
  if (update) {
    SndLfo_Update(&channel->lfo);
  }
  return value;
}
