#pragma opt_propagation off
#include "sdk/card.h"

extern u32 data_022ac0e0; /* offset added to every ROM address */

void OS_SleepThread(OSThreadQueue* queue);
void MI_StopDma(int channel);
void CARDi_SetTask(GameCardTaskStep task);
void func_021154ac(void);
BOOL CARD_WaitRomAsync(void);
BOOL func_021162d8(CARDiRomPage* page);
void CardRom_TaskReadRest(CARDiCommon* common);

/* Reads len bytes of game card ROM at src into dst; dma is a DMA channel (0 to 3) or another value for CPU copies. */
void CARDi_ReadRom(u32 dma, u32 src, u32 dst, u32 len, CARDCallback callback,
                   void* callbackArg, BOOL isAsync) {
  CARDiCommon* common = &data_022abb00;
  CARDiRomPage* page = &data_022ac100;
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
      CARD_WaitRomAsync();
    }
    return;
  }
  if (isAsync) {
    CARDi_SetTask(CardRom_TaskReadRest);
    return;
  }
  common->waiter = OSi_ThreadInfo.current;
  CardRom_TaskReadRest(common);
}
