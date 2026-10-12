#include "sdk/snd_seq.h"

/* Volume of each 0-127 level in 1/10 dB: -723 for 0 up to 0 for 127. */
extern s16 data_arm7_03807180[128];

/*
 * Moves the envelope one tick when update is set (attack, then decay down to the sustain level, then hold;
 * release falls until the sound is stopped) and returns its attenuation in 1/10 dB steps.
 */
int SndExChannel_UpdateEnvelope(SndExChannel* channel, BOOL update) {
  if (update) {
    switch (channel->envStatus) {
      case ENV_STAGE_RISE:
        channel->envAttenuation =
            -((-channel->envAttenuation * channel->attack) >> 8);
        if (channel->envAttenuation == 0) {
          channel->envStatus = ENV_STAGE_FALL;
        }
        break;
      case ENV_STAGE_FALL: {
        int sustain = data_arm7_03807180[channel->sustain] << 7;

        channel->envAttenuation -= channel->decay;
        if (channel->envAttenuation <= sustain) {
          channel->envAttenuation = sustain;
          channel->envStatus = ENV_STAGE_HOLD;
        }
        break;
      }
      case ENV_STAGE_HOLD:
        break;
      case ENV_STAGE_FADE:
        channel->envAttenuation -= channel->release;
        break;
    }
  }
  return channel->envAttenuation >> 7;
}
