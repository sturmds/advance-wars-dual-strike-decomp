#include "sdk/snd_seq.h"

void func_arm7_037ff3b4(SndChannelLfo *lfo); /* clears the oscillator counters */

/* Envelope level a note starts from: the lowest channel volume, -723, in 1/128 steps. */
#define ENV_LEVEL_SILENT (-723 * 128)

/* Starts a note on the channel: envelope from silence at stage 0, LFO restarted, then flags it to start and active. */
void SndExChannel_Start(SndExChannel *channel, int length)
{
    channel->envAttenuation = ENV_LEVEL_SILENT;
    channel->envStatus = 0;
    channel->length = length;
    func_arm7_037ff3b4(&channel->lfo);
    channel->start = TRUE;
    channel->active = TRUE;
}
