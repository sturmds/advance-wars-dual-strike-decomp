#include "types.h"
#include "game/Unk_022a7ea8.h"

void func_0202244c(void *sprite);
void Mem_Free(void *block);

void NameEntry_FreeButtonSprites(NameEntry *entry)
{
    func_0202244c(entry->buttonSprites[0]);
    func_0202244c(entry->buttonSprites[1]);
    func_0202244c(entry->buttonSprites[2]);
    Mem_Free(entry->buttonSpriteFile);
}
