#include "types.h"

void *SoundArc_GetFileAddress(int fileId);
void *SoundArc_GetCurrent(void);
void SoundArc_SetFileAddress(int a0, int a1);
void SoundArc_UnloadItem(void);
typedef void (*SoundLoadFn)(void);

void *SoundArc_LoadFileToHeap(int fileId, SoundLoadFn loader, void *cacheOwner, int fileId2, void *heap);

/* Returns the sound archive file fileId when it is already in memory.
 * Otherwise loads it into heap. A cached load is recorded in the cache
 * table. Returns NULL when the load fails. */
void *func_02107204(int fileId, void *heap, int cache)
{
    void *loaded = SoundArc_GetFileAddress(fileId);
    void *cacheOwner;

    if (loaded == NULL) {
        if (cache != 0) {
            cacheOwner = SoundArc_GetCurrent();
        } else {
            cacheOwner = NULL;
        }
        loaded = SoundArc_LoadFileToHeap(fileId, SoundArc_UnloadItem, cacheOwner, fileId, heap);
        if (cache != 0 && loaded != NULL) {
            SoundArc_SetFileAddress(fileId, (int)loaded);
        }
    }
    return loaded;
}
