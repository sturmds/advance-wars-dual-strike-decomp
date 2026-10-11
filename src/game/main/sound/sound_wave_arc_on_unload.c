#include "types.h"
#include "sdk/snd.h"
#include "game/sound_arc.h"

void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo);

/* Unload callback of the wave archives that SoundArc_LoadWaveArcTable puts in
 * a sound heap: forgets the archive's address in arc, then passes it to
 * SND_DestroyWaveArc. */
void SoundWaveArc_OnUnload(void *waveArc, u32 size, SoundArc *arc, int fileId)
{
    SoundArc_ForgetLoadedItem(waveArc, arc, fileId);
    SND_DestroyWaveArc(waveArc);
}
