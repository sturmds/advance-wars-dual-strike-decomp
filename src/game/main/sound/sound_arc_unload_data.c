#include "types.h"
#include "game/sound_arc.h"

void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo);
void SND_InvalidateSeqData(void *start, void *end);

/* Unload callback of plain files loaded by SoundArc_LoadFileToHeap: forgets
 * the item, then passes the freed range to SND_InvalidateSeqData. */
void SoundArc_UnloadData(void *loaded, int size, SoundArc *arc, int itemNo)
{
    SoundArc_ForgetLoadedItem(loaded, arc, itemNo);
    SND_InvalidateSeqData(loaded, (u8 *)loaded + size);
}
