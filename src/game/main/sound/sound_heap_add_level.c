#include "types.h"
#include "game/sound_heap.h"

void* FrameHeap_Alloc(void* frameHeap, int size, int align);
void func_02106a5c(LinkList* list);

/* Adds an empty save point to the heap's level list. Returns FALSE when the
 * frame heap has no room for it. */
BOOL SoundHeap_AddLevel(SoundHeap* heap) {
  LinkList* level = FrameHeap_Alloc(heap->frameHeap, 0x14, 4);

  if (level == NULL) {
    return FALSE;
  }
  func_02106a5c(level);
  LinkList_Append(&heap->levels, level);
  return TRUE;
}
