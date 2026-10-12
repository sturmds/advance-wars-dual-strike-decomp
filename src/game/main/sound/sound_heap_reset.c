#include "types.h"
#include "game/sound_heap.h"

void FrameHeap_Free(void* frameHeap, int flags);
void func_021069a8(void);

/* Drops every save point of the sound heap, running the unload callback of
 * each item in them, then empties the frame heap and starts again with one
 * empty level. */
void SoundHeap_Reset(SoundHeap* heap) {
  LinkList* saved;
  SoundHeapItem* item;

  saved = LinkList_GetPrev(&heap->levels, NULL);
  while (saved != NULL) {
    item = LinkList_GetPrev(saved, NULL);
    while (item != NULL) {
      if (item->onUnload != NULL) {
        item->onUnload(item->payload, item->unk_08, item->unk_10, item->unk_14);
      }
      item = LinkList_GetPrev(saved, item);
    }
    LinkList_Remove(&heap->levels, saved);
    saved = LinkList_GetPrev(&heap->levels, NULL);
  }
  FrameHeap_Free(heap->frameHeap, 3);
  func_021069a8();
  SoundHeap_AddLevel(heap);
}
