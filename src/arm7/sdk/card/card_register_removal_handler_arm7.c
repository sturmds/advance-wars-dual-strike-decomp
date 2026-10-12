#include "types.h"
#include "sdk/card.h"
#include "sdk/pxi.h"

void func_arm7_037fe0a0(void);                                  /* PXI_Init */
BOOL func_arm7_037fe270(u32 tag, u32 proc);                     /* PXI_IsCallbackReady */
void func_arm7_037fe298(u32 tag, PXIFifoCallback callback);     /* PXI_SetFifoRecvCallback */
void Card_OnRemovalMessage_Arm7(u32 tag, u32 data, int err);

extern BOOL gCardPullOutInitDone;

/* Registers the card removal receive callback once the ARM9 has registered its own. */
void Card_RegisterRemovalHandler_Arm7(void)
{
    if (!gCardPullOutInitDone) {
        gCardPullOutInitDone = TRUE;
        func_arm7_037fe0a0();
        while (!func_arm7_037fe270(FIFO_TAG_CARD_REMOVAL, PXI_PROC_ARM9)) {
        }
        func_arm7_037fe298(FIFO_TAG_CARD_REMOVAL, Card_OnRemovalMessage_Arm7);
    }
}
