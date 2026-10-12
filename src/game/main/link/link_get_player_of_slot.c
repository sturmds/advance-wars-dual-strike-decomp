#include "types.h"

/* Player index + 1 for each link slot; 0: empty. */
extern u8 gLinkSlotPlayers[];

/* Returns the player index in link slot (1-4), or -1 for an empty slot.
 * The reverse of Link_FindSlotOfPlayer. */
int Link_GetPlayerOfSlot(int slot) { return gLinkSlotPlayers[slot - 1] - 1; }
