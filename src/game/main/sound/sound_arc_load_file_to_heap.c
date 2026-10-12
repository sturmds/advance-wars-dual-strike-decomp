#include "types.h"
#include "game/sound_arc.h"
#include "game/sound_heap.h"
#include "sdk/os.h"

u32 func_021063d0(u32 fileNo);
s32 SoundArc_ReadFile(u32 fileNo, void *dest, u32 size, u32 offset);
void *SoundHeap_AllocItem(SoundHeap *heap, u32 size, void (*onUnload)(void *payload, u32 arg1, u32 arg2, u32 arg3), u32 arg3, u32 arg4);

/* Reads file fileNo of the current sound archive into a new item on heap.
 * onUnload, owner and itemNo go to the item for its unload callback. Returns
 * the data, or NULL when the file is empty, there is no heap, the heap is full
 * or the read falls short. */
void *SoundArc_LoadFileToHeap(u32 fileNo, void (*onUnload)(void *payload, u32 arg1, u32 arg2, u32 arg3), SoundArc *owner, u32 itemNo, SoundHeap *heap)
{
    u32 size = func_021063d0(fileNo);
    u8 *data;

    if (size == 0) {
        return NULL;
    }
    if (heap == NULL) {
        return NULL;
    }
    data = SoundHeap_AllocItem(heap, size + 32, onUnload, (u32)owner, itemNo);
    if (data == NULL) {
        return NULL;
    }
    if (size != SoundArc_ReadFile(fileNo, data, size, 0)) {
        return NULL;
    }
    DC_StoreRange(data, size);
    return data;
}
