#include "sdk/snd_seq.h"

int func_arm7_037ff300(SndChannelLfo *lfo);  /* current oscillator value */
void func_arm7_037ff350(SndChannelLfo *lfo); /* advances the oscillator */

/* Returns the channel's LFO offset scaled for its target (pitch, volume or pan), and advances the LFO when update is set. */
int SndExChannel_UpdateLfo(SndExChannel *channel, BOOL update)
{
    s64 value = func_arm7_037ff300(&channel->lfo);

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
        func_arm7_037ff350(&channel->lfo);
    }
    return value;
}
