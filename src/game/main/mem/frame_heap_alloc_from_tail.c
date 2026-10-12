#include "types.h"
#include "game/heap.h"

void MIi_CpuClear32(u32 value, void* dest,
                    u32 size); /* fills [dest, dest + size) with value */

/* Takes size bytes from the end of the free region, with the start of the
 * block rounded down to align (a power of two). Returns NULL when the region
 * is too small. */
void* FrameHeap_AllocFromTail(FrameHeapData* data, u32 size, int align) {
  void* oldEnd = data->free.end;
  u32 start = ~(align - 1) & ((u32)oldEnd - size);
  volatile u32 fill;
  u32 fillSize;

  if (start < (u32)data->free.start) {
    return NULL;
  }
  fillSize = (u32)oldEnd - start;
  /* option bit 0: fill the taken bytes with 0 */
  if ((u16)(((FrameHeap*)((u32)data - sizeof(HeapHead)))->head.options & 0xFF) &
      1) {
    fill = 0;
    MIi_CpuClear32(fill, (void*)start, fillSize);
  }
  data->free.end = (void*)start;
  return (void*)start;
}
