#include "types.h"
#include "game/heap.h"

/* Creates a frame heap in [start, start + size), both rounded to 4 bytes.
 * Returns NULL when fewer than 0x30 bytes are left. */
FrameHeap* FrameHeap_Create(u32 start, u32 size, u16 options) {
  u32 end = size + start;

  start += 3;
  end &= ~3;
  start &= ~3;
  if (start > end || end - start < 0x30) {
    return NULL;
  }
  return FrameHeap_Init((void*)start, (void*)end, options);
}
