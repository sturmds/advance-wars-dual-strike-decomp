/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

void OS_SleepThread(OSThreadQueue* queue);

/* Takes the card lock for owner and target, waiting while another owner holds it; nested locks are counted. */
void CARDi_LockResource(int owner, int target) {
  CARDiCommon* common = &data_022abb00;
  int state = OS_DisableInterrupts();

  if (common->lockOwner == owner) {
    if (common->lockTarget != target) {
      OS_Terminate();
    }
  } else {
    while (common->lockOwner != OS_LOCK_ID_ERROR) {
      OS_SleepThread(&common->lockQueue);
    }
    common->lockOwner = owner;
    common->lockTarget = target;
  }
  common->lockCount++;
  common->cmd->result = CARD_RESULT_SUCCESS;
  OS_RestoreInterrupts(state);
}
