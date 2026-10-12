#include "sdk/snd_seq.h"
#include "sdk/snd.h"

/* Current oscillator output: 0 until the start delay has passed. */
int SndLfo_GetValue(SndChannelLfo *lfo)
{
    if (lfo->param.depth == 0) {
        return 0;
    }
    if (lfo->delayCounter < lfo->param.delay) {
        return 0;
    }
    /* needed to match: the u32 cast makes the shift logical */
    return SndDrv_SineWave((u32)lfo->counter >> 8) * lfo->param.depth * lfo->param.range;
}
