/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

void OS_WakeupThread(OSThreadQueue* queue);
void OS_WakeupThreadDirect(OSThread* thread);

/* Ends the running request: reports success, wakes the waiters and calls the callback. */
void CARDi_EndTask(void) {
  CARDiCommon* common = &data_022abb00;
  CARDCallback callback = common->callback;
  void* callbackArg = common->callbackArg;
  int state;

  common->cmd->result = CARD_RESULT_SUCCESS;
  state = OS_DisableInterrupts();
  common->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK);
  OS_WakeupThread(&common->queue);
  if (common->flag & GAMECARD_FLAG_WORKER_SLEEPS) {
    OS_WakeupThreadDirect(&common->thread);
  }
  OS_RestoreInterrupts(state);
  if (callback) {
    callback(callbackArg);
  }
}
