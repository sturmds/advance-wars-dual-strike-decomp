#include "types.h"
#include "sdk/pxi.h"
#include "sdk/os_arm7.h"

/* Outcomes of reading one word from the receive FIFO, besides a word. */
#define FIFO_READ_FAULT (-3)
#define FIFO_READ_NOTHING (-4)

/* The ARM7 receive callback per tag. */
extern PXIFifoCallback data_arm7_03807ab8[FIFO_TAG_LIMIT];

/* The ARM7 Fifo_DispatchReceived: reads every queued word and passes
 * it to the callback of its tag. A word with an unknown tag is sent back
 * with the error bit set, unless it already has the bit set. Tag 0 is
 * ignored. */
void Fifo_DispatchReceived_Arm7(void) {
  PXIFifoMessage message;
  int status;
  int irq;

  for (;;) {
    if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
      *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
      status = FIFO_READ_FAULT;
    } else {
      irq = Cpu_MaskIrq_Arm7();
      if (*IO_FIFO_CTRL & FIFO_CTRL_INBOX_EMPTY) {
        Cpu_RestoreIrqBit_Arm7(irq);
        status = FIFO_READ_NOTHING;
      } else {
        *(u32*)&message = *IO_FIFO_INBOX;
        Cpu_RestoreIrqBit_Arm7(irq);
        status = 0;
      }
    }
    if (status == FIFO_READ_NOTHING) {
      return;
    }
    if (status == FIFO_READ_FAULT) {
      continue;
    }
    if (message.tag == 0) {
      continue;
    }
    if (data_arm7_03807ab8[message.tag] != NULL) {
      data_arm7_03807ab8[message.tag](message.tag, message.data, message.err);
    } else if (!message.err) {
      message.err = 1;
      if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
        *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
      } else {
        irq = Cpu_MaskIrq_Arm7();
        if (*IO_FIFO_CTRL & FIFO_CTRL_OUTBOX_FULL) {
          Cpu_RestoreIrqBit_Arm7(irq);
        } else {
          *IO_FIFO_OUTBOX = *(u32*)&message;
          Cpu_RestoreIrqBit_Arm7(irq);
        }
      }
    }
  }
}
