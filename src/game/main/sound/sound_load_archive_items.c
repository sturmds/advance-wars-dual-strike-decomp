#include "types.h"

/* Sound archive loaders: each loads one item into the sound heap and
 * returns TRUE on success. */
BOOL func_02107804(int itemNo, void *heap);
BOOL func_021077cc(int itemNo, void *heap);

extern void *gSoundHeap;
extern BOOL gSoundReady;

/* Loads two sound archive items into the sound heap with the same pair of
 * loaders Sound_LoadResidentData uses. */
void Sound_LoadArchiveItems(int firstItem, int secondItem)
{
    if (gSoundReady == 0) {
        return;
    }
    func_02107804(firstItem, gSoundHeap);
    func_021077cc(secondItem, gSoundHeap);
}
