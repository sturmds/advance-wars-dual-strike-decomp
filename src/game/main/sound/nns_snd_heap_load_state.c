#include "types.h"
#include "game/sound.h"
#include "game/sound_heap.h"
#include "game/link_list.h"

int FrameHeap_FreeByState(void* frameHeap, int level);
int FrameHeap_SaveState(void* frameHeap, int level);
void func_021069a8(void);
void SoundHeap_Reset(SoundHeap* heap);

/* NitroSystem NNS_SndHeapLoadState: drops every save point from the newest
 * down to level, running the unload callback of each item in them. Level 0
 * drops everything. */
void NNS_SndHeapLoadState(void* heapArg, int level) {
  SoundHeap* heap = heapArg;
  LinkList* saved;
  SoundHeapItem* item = NULL;

  if (level == 0) {
    SoundHeap_Reset(heap);
    return;
  }
  if (level < heap->levels.count) {
    LinkList* levels = &heap->levels;
    void* none = item; /* NULL; needed to match: a second copy of item */

    do {
      saved = LinkList_GetPrev(levels, none);
      item = LinkList_GetPrev(saved, item);
      while (item != NULL) {
        if (item->onUnload != NULL) {
          item->onUnload(item->payload, item->unk_08, item->unk_10,
                         item->unk_14);
        }
        item = LinkList_GetPrev(saved, item);
      }
      LinkList_Remove(levels, saved);
    } while (level < heap->levels.count);
  }
  FrameHeap_FreeByState(heap->frameHeap, level);
  func_021069a8();
  FrameHeap_SaveState(heap->frameHeap, heap->levels.count);
  SoundHeap_AddLevel(heap);
}
