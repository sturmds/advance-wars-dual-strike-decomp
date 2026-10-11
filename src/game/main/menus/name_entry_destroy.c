/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

void Mem_Free(void *block);
void Wallpaper_DestroyTasks(void);
void NameEntry_FreeButtonSprites(NameEntry *entry);
int NameEntry_FreeCaret(NameEntry *entry);

/* onDestroy of the name entry task: frees the sprites and the work block.
 * The 8-character form (maxLength 8) also destroys the wallpaper tasks. */
void NameEntry_Destroy(NameEntryTask *task)
{
    NameEntry *entry = task->work;

    NameEntry_FreeCaret(entry);
    NameEntry_FreeButtonSprites(entry);
    if (entry->maxLength == 8) {
        Wallpaper_DestroyTasks();
    }
    Mem_Free(entry);
}
