#include "types.h"
#include "sdk/pxi.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* The ARM7 PXI_SendWordByFifo: queues one message for the ARM9. Returns 0
 * on success, FIFO_PUSH_FAULT (and clears the FIFO error) or FIFO_PUSH_NO_ROOM. */
int func_arm7_037fe1cc(u32 tag, u32 data, int err)
{
    PXIFifoMessage message;
    int irq;

    message.tag = tag;
    message.err = err;
    message.data = data;
    if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
        *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
        return FIFO_PUSH_FAULT;
    }
    irq = func_arm7_037fdb80();
    if (*IO_FIFO_CTRL & FIFO_CTRL_OUTBOX_FULL) {
        func_arm7_037fdb94(irq);
        return FIFO_PUSH_NO_ROOM;
    }
    *IO_FIFO_OUTBOX = *(u32 *)&message;
    func_arm7_037fdb94(irq);
    return 0;
}
