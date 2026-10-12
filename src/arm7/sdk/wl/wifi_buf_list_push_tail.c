#include "types.h"
#include "sdk/wl.h"
#include "sdk/os_arm7.h"

#define WIFI_LIST_LOCK_IRQS 0x01000000

/* Adds a buffer at the end of a list and marks it as a WL buffer of that list. Always returns 0. */
int WifiBufList_PushTail(WlBufferList* list, WlBuffer* buffer) {
  u32 irqState = Irq_ClearEnableBits_Arm7(WIFI_LIST_LOCK_IRQS);

  if (list->count == 0) {
    buffer->prev = WIFI_LIST_END;
    list->first = buffer;
  } else {
    WlBuffer* last = list->last;

    buffer->prev = last;
    last->next = buffer;
  }
  buffer->next = WIFI_LIST_END;
  buffer->listId = list->id;
  buffer->magic = WIFI_MSG_BUFFER_TAG;
  list->last = buffer;
  list->count++;
  Irq_SetEnableBits_Arm7(irqState);
  return 0;
}
