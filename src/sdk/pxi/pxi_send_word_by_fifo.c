#include "types.h"
#include "sdk/os.h"
#include "sdk/pxi.h"

/* Queues one message for the other CPU. Returns 0 on success, FIFO_PUSH_FAULT
 * (and clears the FIFO error) or FIFO_PUSH_NO_ROOM. */
int PXI_SendWordByFifo(u32 tag, u32 data, int err) {
  PXIFifoMessage message;
  int irq;

  message.tag = tag;
  message.err = err;
  message.data = data;
  if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
    *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
    return FIFO_PUSH_FAULT;
  }
  irq = OS_DisableInterrupts();
  if (*IO_FIFO_CTRL & FIFO_CTRL_OUTBOX_FULL) {
    OS_RestoreInterrupts(irq);
    return FIFO_PUSH_NO_ROOM;
  }
  *IO_FIFO_OUTBOX = *(u32*)&message;
  OS_RestoreInterrupts(irq);
  return 0;
}
