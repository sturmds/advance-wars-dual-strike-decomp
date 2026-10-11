#include "sdk/card.h"

void OS_SleepThread(OSThreadQueue *queue);

void func_02115688(int owner, int target)
{
    CARDiCommon *common = &data_022abb00;
    int state = OS_DisableInterrupts();

    int current = common->lockOwner;

    if (current == owner) {
        if (common->lockTarget != target) {
            OS_Terminate();
        }
    } else {
        if (common->lockOwner != OS_LOCK_ID_ERROR) {
            do {
                OS_SleepThread(&common->lockQueue);
            } while (common->lockOwner != OS_LOCK_ID_ERROR);
        }
        common->lockOwner = owner;
        common->lockTarget = target;
    }
    common->lockCount++;
    common->cmd->result = CARD_RESULT_SUCCESS;
    OS_RestoreInterrupts(state);
}
