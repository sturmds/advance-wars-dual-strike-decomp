#include "types.h"
#include "game/name_entry_work.h"

void Mem_Free(void *block);
void Wallpaper_DestroyTasks(void);
void NameEntry_FreeButtonSprites(NameEntry *entry);
int func_02100a58(NameEntry *entry);

/* onDestroy of the name entry task: frees the sprites and the work block.
 * The 8-character form (maxLength 8) also destroys the wallpaper tasks. */
void func_020ffb34(NameEntryTask *task)
{
    NameEntry *entry = task->work;

    func_02100a58(entry);
    NameEntry_FreeButtonSprites(entry);
    if (entry->maxLength == 8) {
        Wallpaper_DestroyTasks();
    }
    Mem_Free(task->work);
}
