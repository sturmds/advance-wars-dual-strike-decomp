#include "sdk/card.h"

void OS_WakeupThread(OSThreadQueue* queue);
void OS_WakeupThreadDirect(OSThread* thread);

/* Ends a backup task: clears the busy and task flags, wakes the waiting threads, then runs the callback. */
void CardSave_FinishTaskAndNotify(CARDiCommon* common) {
  CARDCallback callback = common->callback;
  void* callbackArg = common->callbackArg;
  int state = OS_DisableInterrupts();

  common->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK);
  OS_WakeupThread(&common->queue);
  if (common->flag & GAMECARD_FLAG_WORKER_SLEEPS) {
    OS_WakeupThreadDirect(&common->thread);
  }
  OS_RestoreInterrupts(state);

  if (callback != NULL) {
    callback(callbackArg);
  }
}
