#include "sdk/fs.h"
#include "sdk/os.h"
#include "sdk/destructor_chain.h"

/* Debugger support library: removes the overlay's range from the debugger's table. */
int Isd_UnregisterOverlay(MIProcessor target, void* address, u32 size);

static inline u32 OverlayImageSize(const FSOverlayInfo* info) {
  return info->header.ram_size;
}

static inline u32 OverlayTotalSize(const FSOverlayInfo* info) {
  return info->header.ram_size + info->header.bss_size;
}

/* Before an overlay is unloaded: takes every static object whose destructor
 * lies in the overlay's code, or whose storage lies in its bss, off the
 * runtime's destructor list and runs those destructors, then repeats until
 * a pass finds none. */
void Overlay_RunDestructorsInRange(FSOverlayInfo* info) {
  for (;;) {
    DestructorChain* removed = NULL;
    DestructorChain* last = NULL;
    u8* textTop = info->header.ram_address;
    u8* textBottom = textTop + OverlayImageSize(info);
    u8* bssBottom =
        textBottom + (OverlayTotalSize(info) - OverlayImageSize(info));
    DestructorChain* prev;
    DestructorChain* head;
    DestructorChain* entry;
    DestructorChain* next;
    int irqState = OS_DisableInterrupts();

    head = data_022ad540;
    prev = NULL;
    for (entry = head; entry != NULL; entry = next) {
      u8* destructor = (u8*)entry->destructor;
      u8* object;
      next = entry->next;
      object = entry->object;
      if ((destructor >= textTop && destructor < textBottom) ||
          (object >= textBottom && object < bssBottom)) {
        if (last != NULL) {
          last->next = entry;
        } else {
          removed = entry;
        }
        if (head == entry) {
          data_022ad540 = head = next;
        }
        entry->next = NULL;
        last = entry;
        if (prev != NULL) {
          prev->next = next;
        }
      } else {
        prev = entry;
      }
    }
    OS_RestoreInterrupts(irqState);

    if (removed == NULL) {
      break;
    }
    do {
      next = removed->next;
      if (removed->destructor != NULL) {
        removed->destructor(removed->object);
      }
      removed = next;
    } while (removed != NULL);
  }
  {
    /* Needed to match: both locals, in this order. */
    u32 size = OverlayImageSize(info);
    void* address = info->header.ram_address;
    Isd_UnregisterOverlay(info->target == MI_PROCESSOR_ARM9 ? MI_PROCESSOR_ARM9
                                                            : MI_PROCESSOR_ARM7,
                          address, size);
  }
}
