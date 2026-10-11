#include "types.h"
#include "game/sound_handle.h"

/* Current value of a fader: linear from start to target over frames, target
 * once elapsed reaches frames. The casts make the compare and divide signed
 * (the fields are declared u32). */
int SoundFader_GetValue(SoundFader *fader)
{
    int start;

    if ((s32)fader->elapsed >= (s32)fader->frames) {
        return fader->target;
    }
    start = fader->start;
    return start + ((s32)fader->elapsed * (fader->target - start)) / (s32)fader->frames;
}
