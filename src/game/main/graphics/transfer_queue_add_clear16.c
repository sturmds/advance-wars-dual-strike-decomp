#include "types.h"
#include "game/transfer_queue.h"

/* Queues a clear of `size` bytes at `dest` in 16-bit units for the next frame
 * update. Returns the entry number, or -1 when the queue is full. */
s16 TransferQueue_AddClear16(void* dest, u32 size) {
  if (data_0216e04c == TRANSFER_QUEUE_SIZE) {
    return -1;
  }
  data_0216e05c[data_0216e04c].src = dest;
  data_0216e05c[data_0216e04c].size = size;
  data_0216e05c[data_0216e04c].kind = TRANSFER_CLEAR16;
  data_0216e04c++;
  data_0216e050 += size;
  return data_0216e04c - 1;
}
