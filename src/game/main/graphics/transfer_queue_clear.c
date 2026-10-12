#include "types.h"
#include "game/transfer_queue.h"

/* Empties the transfer queue. */
void TransferQueue_Clear(void) {
  u16 i;

  data_0216e04c = 0;
  for (i = 0; i < TRANSFER_QUEUE_SIZE; i++) {
    data_0216e05c[i].src = NULL;
    data_0216e05c[i].dst = NULL;
    data_0216e05c[i].size = 0;
    data_0216e05c[i].kind = 0;
  }
}
