#include "types.h"
#include "game/sound_arc.h"

extern SoundArc *data_022a8a60;             /* current sound archive */

/* Copies the current sound archive's range words to out. */
void SoundArc_GetRange(SoundArcRange *out)
{
    *out = data_022a8a60->range;
}
