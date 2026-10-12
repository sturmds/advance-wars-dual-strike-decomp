#include "types.h"
#include "game/transfer_queue.h"

void Mem_CopyAuto(const void* src, void* dst, u32 size);
int func_01ff9eac(void); /* main loop flag: nonzero while the queue is in use */

/* Queues a copy for the next frame update, or does it now when the flag is
 * 0. Returns the entry number, 0 for an immediate copy, -1 when the queue is full. */
s16 TransferQueue_Add(const void* src, void* dst, u32 size) {
  if (func_01ff9eac() == 0) {
    Mem_CopyAuto(src, dst, size);
    return 0;
  }
  if (data_0216e04c == TRANSFER_QUEUE_SIZE) {
    return -1;
  }
  data_0216e05c[data_0216e04c].src = src;
  data_0216e05c[data_0216e04c].dst = dst;
  data_0216e05c[data_0216e04c].size = size;
  data_0216e05c[data_0216e04c].kind = 0;
  data_0216e04c++;
  data_0216e050 += size;
  return data_0216e04c - 1;
}
