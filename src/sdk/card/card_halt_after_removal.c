#include "sdk/card.h"
#include "sdk/pm.h"
#include "sdk/pad.h"

void Card_PushRemovalTagWord(u32 word, u32 waitCount);

/* The card was removed: powers off if the lid is closed, else tells the other CPU and stops the system. */
void Card_HaltAfterRemoval(void)
{
    if ((*EXTRA_KEYS_MIRROR & EXTRA_KEYS_LID_CLOSED) >> 15) {
        PM_ForceToPowerOff();
    }
    Card_PushRemovalTagWord(1, 1);
    OS_Terminate();
}
