#include "types.h"
#include "sdk/card_arm7.h"
#include "bss_run.h"

static u32 data_arm7_03809870;
#pragma section BSS_RUN1 begin
BOOL gCardRemoved;
u32 gCardPulledOut; /* set once the card was found pulled out */
BOOL gCardPullOutInitDone;
#pragma section BSS_RUN1 end
/* aligned to 32: no symbol covers the bytes before it; also declared in src/arm7/sdk/card/card_rom_begin_cpu_read.c */
CARDiRomPage data_arm7_03809880 __attribute__((aligned(32)));

void CARDi_InitCommon_Arm7(void);
void Card_RegisterRemovalHandler_Arm7(void);
void CardRom_ReadPagesByCpu_Arm7(CARDiRomPage* page);

/* Sets up the ARM7 card state once: clears the request, starts the card thread and installs the ROM page reader. */
void Card_StartService_Arm7(void) {
  CARDiArm7Common* common = &data_arm7_03809280;

  if (common->flag == 0) {
    common->flag = CARD_STAT_INIT;
    common->len = 0;
    common->dst = (u8*)common->len;
    common->src = (u32)common->dst;
    common->dmaNo = -1;
    common->callback = NULL;
    common->callbackArg = NULL;
    data_arm7_03809870 = 0;
    CARDi_InitCommon_Arm7();
    data_arm7_03809880.readPages = CardRom_ReadPagesByCpu_Arm7;
    Card_RegisterRemovalHandler_Arm7();
  }
}
