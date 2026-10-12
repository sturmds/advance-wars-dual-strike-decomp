#include "types.h"
#include "sdk/pxi.h"
#include "sdk/os_arm7.h"

/* The ARM7 PXI_SendWordByFifo: queues one message for the ARM9. Returns 0
 * on success, FIFO_PUSH_FAULT (and clears the FIFO error) or FIFO_PUSH_NO_ROOM. */
int Fifo_PushWord_Arm7(u32 tag, u32 data, int err) {
  PXIFifoMessage message;
  int irq;

  message.tag = tag;
  message.err = err;
  message.data = data;
  if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
    *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
    return FIFO_PUSH_FAULT;
  }
  irq = Cpu_MaskIrq_Arm7();
  if (*IO_FIFO_CTRL & FIFO_CTRL_OUTBOX_FULL) {
    Cpu_RestoreIrqBit_Arm7(irq);
    return FIFO_PUSH_NO_ROOM;
  }
  *IO_FIFO_OUTBOX = *(u32*)&message;
  Cpu_RestoreIrqBit_Arm7(irq);
  return 0;
}
