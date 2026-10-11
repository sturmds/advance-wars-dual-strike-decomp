/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

extern OSThreadInfo OSi_ThreadInfo;

void OS_SleepThread(OSThreadQueue *queue);
void CARDi_SetTask(GameCardTaskStep task);
void CardSave_TaskWriteChunks(CARDiCommon *common);

/* Writes len bytes from src to backup memory at dst; with isAsync, returns once the task is queued. */
BOOL CARDi_WriteBackupCore(u32 dst, u32 src, u32 len, CARDCallback callback, void *callbackArg, BOOL isAsync)
{
    CARDiCommon *common = &data_022abb00;
    int state = OS_DisableInterrupts();

    if (common->flag & CARD_STAT_BUSY) {
        do {
            OS_SleepThread(&common->queue);
        } while (common->flag & CARD_STAT_BUSY);
    }
    common->flag |= CARD_STAT_BUSY;
    OS_RestoreInterrupts(state);

    common->src = src;
    common->dst = dst;
    common->len = len;
    common->callback = callback;
    common->callbackArg = callbackArg;
    if (isAsync) {
        CARDi_SetTask(CardSave_TaskWriteChunks);
        return TRUE;
    }
    data_022abb00.waiter = OSi_ThreadInfo.current;
    CardSave_TaskWriteChunks(common);
    return common->cmd->result == CARD_RESULT_SUCCESS;
}
