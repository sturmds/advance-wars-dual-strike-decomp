#pragma opt_propagation off
#include "sdk/card.h"

extern OSThreadInfo OSi_ThreadInfo;
extern u32 data_022ac0e0;      /* offset added to every ROM address */

void OS_SleepThread(OSThreadQueue *queue);
void MI_StopDma(int channel);
void CARDi_SetTask(GameCardTaskStep task);
void func_021154ac(void);
void func_02116070(void);      /* waits for the running request (the definition's parameters are not used here) */
BOOL func_021162d8(CARDiRomPage *page);
void func_021161a4(CARDiCommon *common);

/* Reads len bytes of game card ROM at src into dst; dma is a DMA channel (0 to 3) or another value for CPU copies. */
void CARDi_ReadRom(u32 dma, u32 src, u32 dst, u32 len, CARDCallback callback, void *callbackArg, BOOL isAsync)
{
    CARDiCommon *common = &data_022abb00;
    CARDiRomPage *page = &data_022ac100;
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

    common->unk_24 = dma;
    common->src = src + data_022ac0e0;
    common->dst = dst;
    common->len = len;
    common->callback = callback;
    common->callbackArg = callbackArg;
    if (dma <= 3) {
        MI_StopDma(dma);
    }
    if (func_021162d8(page)) {
        if (!isAsync) {
            func_02116070();
        }
        return;
    }
    if (isAsync) {
        CARDi_SetTask(func_021161a4);
        return;
    }
    common->waiter = OSi_ThreadInfo.current;
    func_021161a4(common);
}
