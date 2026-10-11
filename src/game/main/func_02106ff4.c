#include "types.h"
#include "game/sound_arc.h"

void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo);
void SND_InvalidateBankData(void *start, void *end);
void func_02110cdc(void *loaded);

void func_02106ff4(void *loaded, int size, SoundArc *arc, int itemNo)
{
    SoundArc_ForgetLoadedItem(loaded, arc, itemNo);
    SND_InvalidateBankData(loaded, (u8 *)loaded + size);
    func_02110cdc(loaded);
}
