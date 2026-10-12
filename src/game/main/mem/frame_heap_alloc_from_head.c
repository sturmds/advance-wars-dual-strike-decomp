#include "types.h"
#include "game/heap.h"

void MIi_CpuClear32(u32 value, void* dest,
                    u32 size); /* fills [dest, dest + size) with value */

/* Takes size bytes from the start of the free region, with the start rounded
 * up to align (a power of two). Returns NULL when the region is too small. */
void* FrameHeap_AllocFromHead(FrameHeapData* data, u32 size, int align) {
  void* oldStart = data->free.start;
  u32 start = ((align - 1) + (u32)oldStart) & ~(align - 1);
  u32 newStart = size + start;
  volatile u32 fill;
  u32 fillSize;

  if (newStart > (u32)data->free.end) {
    return NULL;
  }
  fillSize = newStart - (u32)oldStart;
  /* option bit 0: fill the taken bytes with 0 */
  if ((u16)(((FrameHeap*)((u32)data - sizeof(HeapHead)))->head.options & 0xFF) &
      1) {
    fill = 0;
    MIi_CpuClear32(fill, oldStart, fillSize);
  }
  data->free.start = (void*)newStart;
  return (void*)start;
}
