#include "types.h"
#include "game/sound_heap.h"

void* FrameHeap_Alloc(void* frameHeap, int size, int align);

/* Allocates an item of size bytes (rounded up to 32) on the heap's frame heap
 * and adds it to the newest level. Returns the item's payload, or NULL when
 * the frame heap is full. */
void* SoundHeap_AllocItem(SoundHeap* heap, u32 size,
                          void (*onUnload)(void* payload, u32 arg1, u32 arg2,
                                           u32 arg3),
                          u32 arg3, u32 arg4) {
  SoundHeapItem* item =
      FrameHeap_Alloc(heap->frameHeap, ((size + 31) & ~31) + 32, 32);
  LinkList* level;

  if (item == NULL) {
    return NULL;
  }
  level = LinkList_GetPrev(&heap->levels, NULL);
  item->unk_08 = size;
  item->onUnload = onUnload;
  item->unk_10 = arg3;
  item->unk_14 = arg4;
  LinkList_Append(level, item);
  return item->payload;
}
