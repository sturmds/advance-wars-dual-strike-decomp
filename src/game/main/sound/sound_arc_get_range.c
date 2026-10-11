#include "types.h"
#include "game/sound_arc.h"

extern SoundArc *gCurrentSoundArc;

/* Copies the current sound archive's range words to out. */
void SoundArc_GetRange(SoundArcRange *out)
{
    *out = gCurrentSoundArc->range;
}
