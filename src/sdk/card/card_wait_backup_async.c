/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

void OS_SleepThread(OSThreadQueue* queue);

/* Waits until the running backup request ends; TRUE if it succeeded. */
BOOL CARD_WaitBackupAsync(void) {
  CARDiCommon* common = &data_022abb00;
  int state = OS_DisableInterrupts();

  if (common->flag & CARD_STAT_BUSY) {
    do {
      OS_SleepThread(&common->queue);
    } while (common->flag & CARD_STAT_BUSY);
  }
  OS_RestoreInterrupts(state);
  return common->cmd->result == CARD_RESULT_SUCCESS;
}
