#include "types.h"
#include "game/name_entry_work.h"

void func_0202244c(void *sprite);
void Mem_Free(void *block);

void NameEntry_FreeButtonSprites(NameEntry *entry)
{
    func_0202244c(entry->buttonSprites[0]);
    func_0202244c(entry->buttonSprites[1]);
    func_0202244c(entry->buttonSprites[2]);
    Mem_Free(entry->buttonSpriteFile);
}
