#include "types.h"

int func_0207bd80(void);
int Link_GetPlayerOfSlot(int slot);

/* The player in a setup slot: slot - 1 outside link play, else the player
 * the link assigned to it. */
int VsSetup_GetPlayerOfSlot(int slot)
{
    if (func_0207bd80() == 0) {
        return slot - 1;
    }
    return Link_GetPlayerOfSlot(slot);
}
