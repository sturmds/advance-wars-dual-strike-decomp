#include "types.h"
#include "game/sound_arc.h"

void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo);
void func_0210fb80(void *start, void *end);
void func_02110c90(void *loaded);

void SoundArc_UnloadItem(void *loaded, int size, SoundArc *arc, int itemNo)
{
    SoundArc_ForgetLoadedItem(loaded, arc, itemNo);
    func_0210fb80(loaded, (u8 *)loaded + size);
    func_02110c90(loaded);
}
