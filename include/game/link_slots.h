#ifndef GAME_LINK_SLOTS_H
#define GAME_LINK_SLOTS_H

#include "types.h"

/* Player index + 1 in each link slot (1-4), connected players first;
 * filled by Link_AssignSlots. */
#define LINK_SLOT_COUNT 4
extern u8 gLinkSlotPlayers[];

#endif
