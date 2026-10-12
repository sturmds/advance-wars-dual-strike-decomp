#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/pxi.h"

void Card_OnRemovalMessage_Arm7(u32 tag, u32 data, int err);

extern BOOL gCardPullOutInitDone;

/* Registers the card removal receive callback once the ARM9 has registered its own. */
void Card_RegisterRemovalHandler_Arm7(void)
{
    if (!gCardPullOutInitDone) {
        gCardPullOutInitDone = TRUE;
        Fifo_SetUp_Arm7();
        while (!Fifo_PeerHasHandler_Arm7(FIFO_TAG_CARD_REMOVAL, PXI_PROC_ARM9)) {
        }
        Fifo_RegisterTagHandler_Arm7(FIFO_TAG_CARD_REMOVAL, Card_OnRemovalMessage_Arm7);
    }
}
