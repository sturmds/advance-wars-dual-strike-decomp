#include "types.h"
#include "game/sound_arc.h"
#include "game/sound_heap.h"

#define SOUND_SLOT_COUNT 32

const void *func_02106484(int index);
void *func_021066dc(void);
void SoundSlot_SetMaxPlayers(int slotNo, int maxPlayers);
void SoundSlot_SetTrackChannelMask(int slotNo, u32 mask);
BOOL SoundPlayerHeap_Create(int slotNo, SoundHeap *heap, u32 size);

/* Sets up the player groups listed in the sound archive: each group gets its
 * player limit, its track channel mask and maxPlayers player heaps taken
 * from heap. Returns FALSE when a heap does not fit. */
BOOL Sound_CreatePlayerHeaps(SoundHeap *heap)
{
    int slotNo;
    const SoundArcPlayerGroupInfo *group;
    int i;

    func_021066dc();
    for (slotNo = 0; slotNo < SOUND_SLOT_COUNT; slotNo++) {
        group = func_02106484(slotNo);
        if (group == NULL) {
            continue;
        }
        SoundSlot_SetMaxPlayers(slotNo, group->maxPlayers);
        SoundSlot_SetTrackChannelMask(slotNo, group->trackChannelMask);
        if (group->heapSize == 0 || heap == NULL) {
            continue;
        }
        for (i = 0; i < group->maxPlayers; i++) {
            if (!SoundPlayerHeap_Create(slotNo, heap, group->heapSize)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
