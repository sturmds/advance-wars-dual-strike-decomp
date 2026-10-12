#include "types.h"
#include "sdk/wl.h"
#include "sdk/os_arm7.h"

#define WIFI_MSG_BUFFER_HEAD_BYTES 12

/* Allocates a buffer with size bytes of payload and adds it to a list. NULL if size is 0 or nothing is left. */
WlBuffer* WifiBuffer_New(WlBufferList* list, int size) {
  WlBufferAllocator* allocator = &WIFI_DRIVER_STATE->allocator;
  WlBuffer* buffer;
  s32 totalSize;

  if (size == 0) {
    return NULL;
  }
  totalSize = size + WIFI_MSG_BUFFER_HEAD_BYTES;
  /* needed to match: an unknown kind leaves buffer uninitialised, as in the original */
  switch (allocator->kind) {
    case 0:
      buffer = Heap_TakeFirstFit_Arm7(allocator->a.arena, allocator->b.heap,
                                      totalSize);
      break;
    case 1:
      buffer = allocator->a.alloc(totalSize);
      break;
  }
  if (buffer == NULL) {
    return NULL;
  }
  buffer->magic = WIFI_MSG_BUFFER_TAG;
  buffer->listId = 0;
  WifiBufList_PushTailIfLoose(list, buffer);
  return buffer;
}
