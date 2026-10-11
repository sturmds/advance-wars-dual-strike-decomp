#include "types.h"

void *func_02106304(int fileId);
void *func_021066dc(void);
void func_021062e8(int a0, int a1);
void func_02106ff4(void);
typedef void (*SoundLoadFn)(void);
void *SoundArc_LoadFileToHeap(int fileId, SoundLoadFn loader, void *cacheOwner, int fileId2, void *heap);

void *func_02107280(int fileId, void *heap, int cache)
{
    void *loaded = func_02106304(fileId);
    void *cacheOwner;

    if (loaded == NULL) {
        if (cache != 0) {
            cacheOwner = func_021066dc();
        } else {
            cacheOwner = NULL;
        }
        loaded = SoundArc_LoadFileToHeap(fileId, func_02106ff4, cacheOwner, fileId, heap);
        if (cache != 0 && loaded != NULL) {
            func_021062e8(fileId, (int)loaded);
        }
    }
    return loaded;
}
