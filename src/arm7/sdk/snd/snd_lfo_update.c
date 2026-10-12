#include "sdk/snd_seq.h"

/* One tick: count down the start delay, then advance the phase by speed / 4 sine steps. */
void SndLfo_Update(SndChannelLfo *lfo)
{
    if (lfo->delayCounter < lfo->param.delay) {
        lfo->delayCounter++;
    } else {
        u32 phase = lfo->counter;

        phase += lfo->param.speed << 6;
        phase >>= 8;
        while (phase >= 128) {
            phase -= 128;
        }
        lfo->counter += lfo->param.speed << 6;
        lfo->counter &= 0xFF;
        lfo->counter |= phase << 8;
    }
}
