#include "types.h"
#include "sdk/os.h"
#include "sdk/pxi.h"

extern u16 gPxiInitialized;

/* Runs once: clears the callback table, enables the receive FIFO interrupt
 * and synchronises with the ARM7 over the PXI sync register. */
void PXI_InitFifo(void)
{
    int irq = OS_DisableInterrupts();
    int i;
    int wait;
    u32 seen;
    int attempts;
    volatile FifoPeerRegistry *work = FIFO_PEER_REGISTRY;

    if (!gPxiInitialized) {
        gPxiInitialized = TRUE;
        work->callbackFlags[PXI_PROC_ARM9] = 0;
        for (i = 0; i < FIFO_TAG_LIMIT; i++) {
            gPxiRecvCallbacks[i] = NULL;
        }
        *IO_FIFO_CTRL = FIFO_CTRL_ON | FIFO_CTRL_FAULT_ACK | FIFO_CTRL_INBOX_IRQ | FIFO_CTRL_FLUSH_OUTBOX;
        OS_ResetRequestIrqMask(IRQ_FIFO_RECV);
        OS_SetIrqFunction(IRQ_FIFO_RECV, Fifo_DispatchReceived);
        OS_EnableIrqMask(IRQ_FIFO_RECV);
        /* Echo the ARM7's 4-bit sync value back until it reads 0 after more
         * than 4 changes; an ARM7 value that does not change in 1000 reads
         * restarts the count. */
        for (attempts = 0;; attempts++) {
            seen = *REG_PXI_SYNC & 15;

            *REG_PXI_SYNC = seen << 8;
            if (seen == 0 && attempts > 4) {
                break;
            }
            wait = 1000;
            while ((*REG_PXI_SYNC & 15) == seen) {
                if (wait <= 0) {
                    attempts = 0;
                    break;
                }
                wait--;
            }
        }
    }
    OS_RestoreInterrupts(irq);
}
