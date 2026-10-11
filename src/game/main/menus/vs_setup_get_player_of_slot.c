#include "types.h"

int Link_IsActive(void);
int Link_GetPlayerOfSlot(int slot);

/* The player in a setup slot: slot - 1 outside link play, else the player
 * the link assigned to it. */
int VsSetup_GetPlayerOfSlot(int slot)
{
    if (Link_IsActive() == 0) {
        return slot - 1;
    }
    return Link_GetPlayerOfSlot(slot);
}
