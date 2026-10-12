#include "types.h"
#include "sdk/card.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void Thread_WaitOnQueue_Arm7(void *queue);                 /* looks like OS_SleepThread */
void Thread_ReadyAllWaiters_Arm7(void *queue);                 /* looks like OS_WakeupThread */
void Thread_MakeReady_Arm7(void *thread);                /* looks like OS_WakeupThreadDirect */
int func_arm7_037fe1cc(u32 tag, u32 data, int err);   /* looks like PXI_SendWordByFifo */

/* Request handlers, indexed by CARDiArm7Common.command. */
extern void (*const data_arm7_038072b0[])(CARDiArm7Common *common);

/*
 * Card thread. Sleeps until the other CPU sends a request or a task is set.
 * A request runs its handler, then the thread answers on PXI and wakes the
 * threads waiting for the card. A task runs common->task.
 */
void Card_ServiceThread_Arm7(void *arg)
{
    CARDiArm7Common *common = &data_arm7_03809280;

    while (TRUE) {
        BOOL request = FALSE;
        int interrupts = func_arm7_037fdb80();

        while (TRUE) {
            if (!(common->flag & CARD_STAT_BUSY)) {
                if (common->flag & GAMECARD_FLAG_PEER_JOB_PENDING) {
                    common->flag |= CARD_STAT_BUSY;
                    common->flag &= ~GAMECARD_FLAG_PEER_JOB_PENDING;
                    request = TRUE;
                    break;
                }
            } else if (common->flag & CARD_STAT_TASK) {
                break;
            }
            common->waiter = common->thread;
            Thread_WaitOnQueue_Arm7(NULL);
        }
        func_arm7_037fdb94(interrupts);

        if (request) {
            common->cmd->result = CARD_RESULT_SUCCESS;
            data_arm7_038072b0[common->command](common);
            while (func_arm7_037fe1cc(FIFO_TAG_CARD_SERVICE, 1, 1) < 0) {
            }
            interrupts = func_arm7_037fdb80();
            common->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK);
            Thread_ReadyAllWaiters_Arm7(&common->queue);
            if (common->flag & GAMECARD_FLAG_PEER_JOB_PENDING) {
                Thread_MakeReady_Arm7(common->thread);
            }
            func_arm7_037fdb94(interrupts);
        } else {
            common->task(common);
        }
    }
}
