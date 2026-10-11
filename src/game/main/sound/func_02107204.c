#include "types.h"

void *func_02106304(int fileId);
void *func_021066dc(void);
void func_021062e8(int a0, int a1);
void SoundArc_UnloadItem(void);
typedef void (*SoundLoadFn)(void);

void *func_021073f4(int fileId, SoundLoadFn loader, void *cacheOwner, int fileId2, void *heap);

/* Returns the sound archive file fileId when it is already in memory.
 * Otherwise loads it into heap. A cached load is recorded in the cache
 * table. Returns NULL when the load fails. */
void *func_02107204(int fileId, void *heap, int cache)
{
    void *loaded = func_02106304(fileId);
    void *cacheOwner;

    if (loaded == NULL) {
        if (cache != 0) {
            cacheOwner = func_021066dc();
        } else {
            cacheOwner = NULL;
        }
        loaded = func_021073f4(fileId, SoundArc_UnloadItem, cacheOwner, fileId, heap);
        if (cache != 0 && loaded != NULL) {
            func_021062e8(fileId, (int)loaded);
        }
    }
    return loaded;
}
