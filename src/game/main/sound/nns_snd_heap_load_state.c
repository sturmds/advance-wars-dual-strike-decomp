#include "types.h"
#include "game/sound.h"
#include "game/sound_heap.h"

void *func_02104124(LinkList *list, void *object);   /* previous object in list, last one for NULL */
int func_02104ab4(void *frameHeap, int level);
int func_02104b10(void *frameHeap, int level);
void func_021069a8(void);
int func_021069cc(SoundHeap *heap);
void func_02106c18(SoundHeap *heap);

/* NitroSystem NNS_SndHeapLoadState: drops every save point from the newest
 * down to level, running the unload callback of each item in them. Level 0
 * drops everything. */
void NNS_SndHeapLoadState(void *heapArg, int level)
{
    SoundHeap *heap = heapArg;
    LinkList *saved;
    SoundHeapItem *item = NULL;

    if (level == 0) {
        func_02106c18(heap);
        return;
    }
    if (level < heap->levels.count) {
        LinkList *levels = &heap->levels;
        void *none = item;   /* NULL; needed to match: a second copy of item */

        do {
            saved = func_02104124(levels, none);
            item = func_02104124(saved, item);
            while (item != NULL) {
                if (item->onUnload != NULL) {
                    item->onUnload(item->payload, item->unk_08, item->unk_10, item->unk_14);
                }
                item = func_02104124(saved, item);
            }
            LinkList_Remove(levels, saved);
        } while (level < heap->levels.count);
    }
    func_02104ab4(heap->frameHeap, level);
    func_021069a8();
    func_02104b10(heap->frameHeap, heap->levels.count);
    func_021069cc(heap);
}
