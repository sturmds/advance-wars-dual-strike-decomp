#include "types.h"
#include "game/sound_handle.h"

/* Current value of a fader: linear from start to target over frames, target
 * once elapsed reaches frames. */
int SoundFader_GetValue(SoundFader *fader)
{
    int start;

    if (fader->elapsed >= fader->frames) {
        return fader->target;
    }
    start = fader->start;
    return start + (fader->elapsed * (fader->target - start)) / fader->frames;
}
