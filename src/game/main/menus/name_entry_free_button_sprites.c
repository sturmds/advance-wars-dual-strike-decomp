#include "types.h"
#include "game/name_entry_work.h"

void SpriteAnim_Free(void *sprite);
void Mem_Free(void *block);

void NameEntry_FreeButtonSprites(NameEntry *entry)
{
    SpriteAnim_Free(entry->buttonSprites[0]);
    SpriteAnim_Free(entry->buttonSprites[1]);
    SpriteAnim_Free(entry->buttonSprites[2]);
    Mem_Free(entry->buttonSpriteFile);
}
