#include "sdk/snd.h"

void SndTrack_Start(SndTrack *track, u8 *base, u32 offset)
{
    track->base = base;
    track->cursor = track->base + offset;
}
