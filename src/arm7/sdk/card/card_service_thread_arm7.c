#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

/* Request handlers, indexed by CARDiArm7Common.command. */
extern void (*const data_arm7_038072b0[])(CARDiArm7Common* common);

/*
 * Card thread. Sleeps until the other CPU sends a request or a task is set.
 * A request runs its handler, then the thread answers on PXI and wakes the
 * threads waiting for the card. A task runs common->task.
 */
void Card_ServiceThread_Arm7(void* arg) {
  CARDiArm7Common* common = &data_arm7_03809280;

  while (TRUE) {
    BOOL request = FALSE;
    int interrupts = Cpu_MaskIrq_Arm7();

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
      common->waiter = &common->thread;
      Thread_WaitOnQueue_Arm7(NULL);
    }
    Cpu_RestoreIrqBit_Arm7(interrupts);

    if (request) {
      common->cmd->result = CARD_RESULT_SUCCESS;
      data_arm7_038072b0[common->command](common);
      while (Fifo_PushWord_Arm7(FIFO_TAG_CARD_SERVICE, 1, 1) < 0) {
      }
      interrupts = Cpu_MaskIrq_Arm7();
      common->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK);
      Thread_ReadyAllWaiters_Arm7(&common->queue);
      if (common->flag & GAMECARD_FLAG_PEER_JOB_PENDING) {
        Thread_MakeReady_Arm7(&common->thread);
      }
      Cpu_RestoreIrqBit_Arm7(interrupts);
    } else {
      common->task(common);
    }
  }
}
