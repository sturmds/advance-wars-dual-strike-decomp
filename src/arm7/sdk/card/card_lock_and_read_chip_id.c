#include "types.h"
#include "sdk/card.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fbe9c(CARDiArm7ThreadQueue *queue);
u32 CardRom_IssueChipIdCommand(void);
void Card_ReportDoneAndWake(void);

/* Takes the card, reads the ROM chip ID and ends the request. */
u32 Card_LockAndReadChipId(void)
{
    CARDiArm7Common *common;
    int interrupts;
    u32 id;

    common = &data_arm7_03809280;
    interrupts = func_arm7_037fdb80();
    while (common->flag & CARD_STAT_BUSY) {
        func_arm7_037fbe9c(&common->queue);
    }
    common->flag |= CARD_STAT_BUSY;
    func_arm7_037fdb94(interrupts);
    common->callback = NULL;
    common->callbackArg = NULL;
    id = CardRom_IssueChipIdCommand();
    Card_ReportDoneAndWake();
    return id;
}
