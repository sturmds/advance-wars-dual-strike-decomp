#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

extern u16 data_arm7_03807ab4;

/* The ARM7 receive callback per tag. */
extern PXIFifoCallback data_arm7_03807ab8[FIFO_TAG_LIMIT];

/* The ARM7 PXI_InitFifo. Runs once: clears the callback table, enables the
 * receive FIFO interrupt and synchronises with the ARM9 over the PXI sync
 * register. */
void Fifo_ResetAndHandshake_Arm7(void)
{
    int irq = Cpu_MaskIrq_Arm7();
    int i;
    volatile FifoPeerRegistry *work;

    if (!data_arm7_03807ab4) {
        data_arm7_03807ab4 = TRUE;
        work = FIFO_PEER_REGISTRY;
        work->callbackFlags[PXI_PROC_ARM7] = 0;
        for (i = 0; i < FIFO_TAG_LIMIT; i++) {
            data_arm7_03807ab8[i] = NULL;
        }
        *IO_FIFO_CTRL = FIFO_CTRL_ON | FIFO_CTRL_FAULT_ACK | FIFO_CTRL_INBOX_IRQ | FIFO_CTRL_FLUSH_OUTBOX;
        Irq_AckPending_Arm7(IRQ_FIFO_RECV);
        Irq_InstallHandlers_Arm7(IRQ_FIFO_RECV, Fifo_DispatchReceived_Arm7);
        Irq_SetEnableBits_Arm7(IRQ_FIFO_RECV);
        /* Count down from 8 on the sync register; the ARM9 echoes each value
         * back, and a value it has not echoed after the wait starts the
         * count again. */
        for (i = 8; i >= 0; i--) {
            *REG_PXI_SYNC = i << 8;
            Cpu_BusyWaitCycles_Arm7(1000);
            if ((*REG_PXI_SYNC & 15) != i) {
                i = 8;
            }
        }
    }
    Cpu_RestoreIrqBit_Arm7(irq);
}
