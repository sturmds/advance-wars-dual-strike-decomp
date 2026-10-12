#include "types.h"
#include "sdk/card_arm7.h"

extern u32 data_arm7_03809870;

void CARDi_InitCommon(void);
void Card_RegisterRemovalHandler_Arm7(void);
void CardRom_ReadPagesByCpu_Arm7(CARDiRomPage *page);

extern CARDiRomPage data_arm7_03809880;

/* Sets up the ARM7 card state once: clears the request, starts the card thread and installs the ROM page reader. */
void Card_StartService_Arm7(void)
{
    CARDiArm7Common *common = &data_arm7_03809280;

    if (common->flag == 0) {
        common->flag = CARD_STAT_INIT;
        common->len = 0;
        common->dst = (u8 *)common->len;
        common->src = (u32)common->dst;
        common->dmaNo = -1;
        common->callback = NULL;
        common->callbackArg = NULL;
        data_arm7_03809870 = 0;
        CARDi_InitCommon();
        data_arm7_03809880.readPages = CardRom_ReadPagesByCpu_Arm7;
        Card_RegisterRemovalHandler_Arm7();
    }
}
