#include "types.h"
#include "game/heap.h"

/* Makes a heap in the memory [start, start + size), trimmed to 4-byte
 * bounds. Returns NULL when there is no room for the headers and at least
 * 4 bytes. */
ExpHeap* Heap_Create(void* start, u32 size, u16 options) {
  void* end = (void*)((size + (u32)start) & ~3);

  start = (void*)(((u32)start + 3) & ~3);

  if (start > end ||
      (u32)end - (u32)start < sizeof(ExpHeap) + sizeof(HeapBlock) + 4) {
    return NULL;
  }
  return ExpHeap_Init(start, end, options);
}
