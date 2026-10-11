#include "sdk/card.h"
#include "sdk/pm.h"

void Card_PushRemovalTagWord(u32 word, u32 waitCount);

/* The card was removed: tells the other CPU, then stops the system (or powers off if flagged). */
void Card_HaltAfterRemoval(void)
{
    if ((REMOVAL_HALT_STATUS_WORD & REMOVAL_HALT_POWER_BIT) >> 15) {
        PM_ForceToPowerOff();
    }
    Card_PushRemovalTagWord(1, 1);
    OS_Terminate();
}
