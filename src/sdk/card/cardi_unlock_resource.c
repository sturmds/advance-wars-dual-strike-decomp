#include "sdk/card.h"

void OS_WakeupThread(OSThreadQueue *queue);

void CARDi_UnlockResource(int owner, int target)
{
    CARDiCommon *common = &data_022abb00;
    int state = OS_DisableInterrupts();

    if (common->lockOwner != owner || common->lockCount == 0) {
        OS_Terminate();
    } else {
        if (common->lockTarget != target) {
            OS_Terminate();
        }
        common->lockCount--;
        if (common->lockCount == 0) {
            common->lockOwner = OS_LOCK_ID_ERROR;
            common->lockTarget = 0;
            OS_WakeupThread(&common->lockQueue);
        }
    }
    common->cmd->result = CARD_RESULT_SUCCESS;
    OS_RestoreInterrupts(state);
}
