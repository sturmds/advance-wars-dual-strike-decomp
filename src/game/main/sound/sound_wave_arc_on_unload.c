#include "types.h"
#include "game/sound_arc.h"

void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo);
void func_02110c90(void *waveArc);

/* Unload callback of the wave archives that SoundArc_LoadWaveArcTable puts in
 * a sound heap: forgets the archive's address in arc, then passes it to
 * func_02110c90. */
void SoundWaveArc_OnUnload(void *waveArc, u32 size, SoundArc *arc, int fileId)
{
    SoundArc_ForgetLoadedItem(waveArc, arc, fileId);
    func_02110c90(waveArc);
}
