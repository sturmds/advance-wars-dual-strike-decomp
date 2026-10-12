#include "types.h"
#include "sdk/card.h"

/* looks like OS_CreateThread */
void func_arm7_037fbf60(void *thread, void (*func)(void *), void *arg, void *stackTop, u32 stackSize,
                        u32 priority);
void Thread_MakeReady_Arm7(void *thread);                        /* looks like OS_WakeupThreadDirect */
void func_arm7_037fe298(int tag, void (*callback)(int tag, u32 data, int err));   /* looks like PXI_SetFifoRecvCallback */
void func_arm7_03802974(u32 state);
void Card_ServiceThread_Arm7(void *arg);
void Card_OnRequestTagMessage_Arm7(int tag, u32 data, int err);

#define GAMECARD_WORKER_PRIO_ARM7 4

/*
 * Clears the request and lock state, starts the card thread and listens for
 * the other CPU's requests on PXI tag 11.
 */
void CARDi_InitCommon(void)
{
    CARDiArm7Common *common = &data_arm7_03809280;

    common->lockOwner = OS_LOCK_ID_ERROR;
    common->lockCount = 0;
    common->lockTarget = 0;
    common->cmd = NULL;
    common->recvStep = 0;
    common->lockQueue = 0;
    *(u16 *)&common->queue = 0;           /* an ARM7 thread queue is a 16-bit mask of thread ids */
    common->priority = GAMECARD_WORKER_PRIO_ARM7;
    /* The stack ends where the SPI transfer state starts. */
    func_arm7_037fbf60(common->thread, Card_ServiceThread_Arm7, NULL, &data_arm7_03809860, GAMECARD_WORKER_STACK_BYTES,
                       common->priority);
    Thread_MakeReady_Arm7(common->thread);
    func_arm7_037fe298(FIFO_TAG_CARD_SERVICE, Card_OnRequestTagMessage_Arm7);
    if (*BOOT_ORIGIN_WORD != BOOT_ORIGIN_WIRELESS) {
        func_arm7_03802974(1);
    }
}
