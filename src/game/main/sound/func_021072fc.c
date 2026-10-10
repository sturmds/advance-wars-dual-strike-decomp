#include "types.h"

void *func_02106304(int fileId);
void *func_021066dc(void);
void func_021062e8(int a0, int a1);
void func_02107030(void);
typedef void (*SoundLoadFn)(void);
void *func_021073f4(int fileId, SoundLoadFn loader, void *cacheOwner, int fileId2, void *heap);

void *func_021072fc(int fileId, void *heap, int cache)
{
    void *loaded = func_02106304(fileId);
    void *cacheOwner;

    if (loaded == NULL) {
        if (cache != 0) {
            cacheOwner = func_021066dc();
        } else {
            cacheOwner = NULL;
        }
        loaded = func_021073f4(fileId, func_02107030, cacheOwner, fileId, heap);
        if (cache != 0 && loaded != NULL) {
            func_021062e8(fileId, (int)loaded);
        }
    }
    return loaded;
}
