/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

extern OSThreadInfo OSi_ThreadInfo;

void OS_SleepThread(OSThreadQueue *queue);
void CARDi_SetTask(GameCardTaskStep task);
void func_02115ebc(CARDiCommon *common);

/* Reads len bytes of backup memory at src into dst; with isAsync, returns once the task is queued. */
BOOL CARDi_ReadBackupCore(u32 src, u32 dst, u32 len, CARDCallback callback, void *callbackArg, BOOL isAsync)
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
        CARDi_SetTask(func_02115ebc);
        return TRUE;
    }
    data_022abb00.waiter = OSi_ThreadInfo.current;
    func_02115ebc(common);
    return common->cmd->result == CARD_RESULT_SUCCESS;
}
