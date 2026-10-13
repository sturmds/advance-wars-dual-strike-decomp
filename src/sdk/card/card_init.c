#pragma opt_propagation off
#include "sdk/card.h"

/* offset added to every ROM address; CARD_Init clears it; also declared in src/sdk/card/cardi_init_common.c and 1 more */
u32 data_022ac0e0;
/* aligned to 32: no symbol covers the bytes before it */
__attribute__((aligned(32))) CARDiRomPage data_022ac100;

void CARDi_InitCommon(
    void); /* sets up the lock, the command buffer and the card thread */
void CardRom_ReadPagesByCpu(
    CARDiRomPage* page); /* reads ROM pages with the CPU */
void Card_RegisterRemovalHandler(void);

void CARD_Init(void) {
  CARDiCommon* common = &data_022abb00;

  if (common->flag != 0) {
    return;
  }
  common->flag = CARD_STAT_INIT;
  common->src = common->dst = common->len = 0;
  common->unk_24 = -1;
  common->callback = NULL;
  common->callbackArg = NULL;
  data_022ac0e0 = 0;
  CARDi_InitCommon();
  data_022ac100.readPages = CardRom_ReadPagesByCpu;
  Card_RegisterRemovalHandler();
}
