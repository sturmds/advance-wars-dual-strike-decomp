#include "types.h"
#include "sdk/card.h"

void func_arm7_0380390c(void);   /* stops sound, then the system */
void func_arm7_037fdc10(void);   /* OS_Terminate */

/* PXI receive callback for the card removal tag: shuts down when the ARM9 says so, else stops. */
void Card_OnRemovalMessage_Arm7(u32 tag, u32 data, int err)
{
    if ((data & CARD_PXI_COMMAND_MASK) == GAMECARD_MSG_POWER_DOWN) {
        func_arm7_0380390c();
    } else {
        func_arm7_037fdc10();
    }
}
