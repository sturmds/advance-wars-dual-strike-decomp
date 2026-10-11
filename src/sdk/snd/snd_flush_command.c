#include "sdk/snd.h"
#include "sdk/os.h"

void DC_FlushRange(void *start, u32 length);
int func_02110f88(int tag, u32 data, int err);

/* Sends the queued commands to the ARM7. Returns 1 when sent, 0 when it would
   have to wait and SOUND_SEND_WAIT_IF_FULL is not set. */
int SND_FlushCommand(u32 flags)
{
    int irq = OS_DisableInterrupts();

    if (data_022a9c70 == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (data_022a9c84 >= SOUND_BATCHES_IN_FLIGHT_MAX) {
        if (!(flags & SOUND_SEND_WAIT_IF_FULL)) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        do {
            SND_RecvCommandReply(1);
        } while (data_022a9c84 >= SOUND_BATCHES_IN_FLIGHT_MAX);
    }
    DC_FlushRange(data_022a9f40, sizeof(data_022a9f40));
    if (func_02110f88(SOUND_FIFO_TAG, (u32)data_022a9c70, 0) < 0) {
        if (!(flags & SOUND_SEND_WAIT_IF_FULL)) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        while (func_02110f88(SOUND_FIFO_TAG, (u32)data_022a9c70, 0) < 0) {
            OS_RestoreInterrupts(irq);
            OS_SpinWait(100);
            irq = OS_DisableInterrupts();
        }
    }
    if (flags & SOUND_SEND_THEN_PING) {
        Sound_PingArm7Driver();
    }
    /* batch ring: 9 slots, wraps after slot 8 */
    data_022a9c8c[data_022a9c80] = data_022a9c70;
    data_022a9c80++;
    if (data_022a9c80 > SOUND_BATCHES_IN_FLIGHT_MAX) {
        data_022a9c80 = 0;
    }
    data_022a9c70 = NULL;
    data_022a9c74 = NULL;
    data_022a9c84++;
    data_022a9c88++;
    OS_RestoreInterrupts(irq);
    return 1;
}
