#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/os_arm7.h"

void func_arm7_0380390c(void);   /* stops sound, then the system */

/* PXI receive callback for the card removal tag: shuts down when the ARM9 says so, else stops. */
void Card_OnRemovalMessage_Arm7(u32 tag, u32 data, int err)
{
    if ((data & CARD_PXI_COMMAND_MASK) == GAMECARD_MSG_POWER_DOWN) {
        func_arm7_0380390c();
    } else {
        Cpu_HaltForever_Arm7();
    }
}
