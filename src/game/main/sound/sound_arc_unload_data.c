#include "types.h"
#include "game/sound_arc.h"

void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo);
void func_0210fbd8(void *start, void *end);

/* Unload callback of plain files loaded by SoundArc_LoadFileToHeap: forgets
 * the item, then passes the freed range to func_0210fbd8. */
void SoundArc_UnloadData(void *loaded, int size, SoundArc *arc, int itemNo)
{
    SoundArc_ForgetLoadedItem(loaded, arc, itemNo);
    func_0210fbd8(loaded, (u8 *)loaded + size);
}
