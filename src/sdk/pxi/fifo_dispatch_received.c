#include "types.h"
#include "sdk/os.h"
#include "sdk/pxi.h"


/* Outcomes of reading one word from the receive FIFO, besides a word. */
#define FIFO_READ_FAULT (-3)
#define FIFO_READ_NOTHING (-4)

/* Interrupt handler for "receive FIFO not empty". Reads every queued word and
 * passes it to the callback of its tag. A word with an unknown tag is sent
 * back with the error bit set, unless it already has the bit set. Tag 0 is
 * ignored. */
void Fifo_DispatchReceived(void)
{
    PXIFifoMessage message;
    int status;
    int irq;

    for (;;) {
        if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
            *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
            status = FIFO_READ_FAULT;
        } else {
            irq = OS_DisableInterrupts();
            if (*IO_FIFO_CTRL & FIFO_CTRL_INBOX_EMPTY) {
                OS_RestoreInterrupts(irq);
                status = FIFO_READ_NOTHING;
            } else {
                *(u32 *)&message = *IO_FIFO_INBOX;
                OS_RestoreInterrupts(irq);
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
        if (gPxiRecvCallbacks[message.tag] != NULL) {
            gPxiRecvCallbacks[message.tag](message.tag, message.data, message.err);
        } else if (!message.err) {
            message.err = 1;
            if (*IO_FIFO_CTRL & FIFO_CTRL_FAULT_ACK) {
                *IO_FIFO_CTRL |= FIFO_CTRL_FAULT_ACK | FIFO_CTRL_ON;
            } else {
                irq = OS_DisableInterrupts();
                if (*IO_FIFO_CTRL & FIFO_CTRL_OUTBOX_FULL) {
                    OS_RestoreInterrupts(irq);
                } else {
                    *IO_FIFO_OUTBOX = *(u32 *)&message;
                    OS_RestoreInterrupts(irq);
                }
            }
        }
    }
}
