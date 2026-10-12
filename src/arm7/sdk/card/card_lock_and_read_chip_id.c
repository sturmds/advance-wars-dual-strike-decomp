#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/os_arm7.h"

u32 CardRom_IssueChipIdCommand(void);
void Card_ReportDoneAndWake(void);

/* Takes the card, reads the ROM chip ID and ends the request. */
u32 Card_LockAndReadChipId(void)
{
    CARDiArm7Common *common;
    int interrupts;
    u32 id;

    common = &data_arm7_03809280;
    interrupts = Cpu_MaskIrq_Arm7();
    while (common->flag & CARD_STAT_BUSY) {
        Thread_WaitOnQueue_Arm7(&common->queue);
    }
    common->flag |= CARD_STAT_BUSY;
    Cpu_RestoreIrqBit_Arm7(interrupts);
    common->callback = NULL;
    common->callbackArg = NULL;
    id = CardRom_IssueChipIdCommand();
    Card_ReportDoneAndWake();
    return id;
}
