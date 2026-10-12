#include "types.h"
#include "game/link_reaction_task.h"

u8 Map_GetLoadedPlayerCount(void);

/* Starts at the right edge of the screen, in the row of the sender's slot;
 * the rows of all players together are centered vertically. */
void LinkReactionTask_Init(LinkReactionTask* task) {
  task->x = 256;
  task->y = ((192 - Map_GetLoadedPlayerCount() * 24) >> 1) +
            (task->slot - 1) * 24 + 4;
}
