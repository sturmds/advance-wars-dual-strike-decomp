#pragma opt_propagation off
#include "sdk/card.h"

extern u32 data_022ac0e0;      /* offset added to every ROM address; CARD_Init clears it */

void func_021154e8(void);      /* sets up the lock, the command buffer and the card thread */
void CardRom_ReadPagesByCpu(CARDiRomPage *page);   /* reads ROM pages with the CPU */
void Card_RegisterRemovalHandler(void);

void CARD_Init(void)
{
    CARDiCommon *common = &data_022abb00;

    if (common->flag != 0) {
        return;
    }
    common->flag = CARD_STAT_INIT;
    common->src = common->dst = common->len = 0;
    common->unk_24 = -1;
    common->callback = NULL;
    common->callbackArg = NULL;
    data_022ac0e0 = 0;
    func_021154e8();
    data_022ac100.readPages = CardRom_ReadPagesByCpu;
    Card_RegisterRemovalHandler();
}
