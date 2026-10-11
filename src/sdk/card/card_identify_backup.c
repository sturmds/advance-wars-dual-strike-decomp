/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

extern OSThreadInfo OSi_ThreadInfo;

void OS_SleepThread(OSThreadQueue *queue);
void func_021154ac(void);
void func_02115858(int type);
int func_021166b0(CARDiCommon *common, int command, int retries);
void func_02115f78(CARDiCommon *common);

/* Sets the backup chip geometry from its type and asks the other CPU to identify the chip. */
void CARD_IdentifyBackup(int type)
{
    CARDiCommon *common = &data_022abb00;
    int state;

    func_021154ac();
    state = OS_DisableInterrupts();
    if (common->flag & CARD_STAT_BUSY) {
        do {
            OS_SleepThread(&common->queue);
        } while (common->flag & CARD_STAT_BUSY);
    }
    common->flag |= CARD_STAT_BUSY;
    OS_RestoreInterrupts(state);

    func_02115858(type);
    common->callback = NULL;
    common->callbackArg = NULL;
    data_022abb00.waiter = OSi_ThreadInfo.current;
    func_021166b0(common, 2, 1);
    func_02115f78(common);
}
