#include "types.h"
#include "game/link_reaction_task.h"
#include "game/link_reaction_queue.h"

int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);

/* gLinkReactionCooldown is the 200 frame wait that LinkReaction_StartCooldown sets after a send. */
void LinkReactionTask_ClearOwnCooldown(LinkReactionTask* task) {
  if (task->slot == Link_FindSlotOfPlayer(Link_GetOwnPlayer())) {
    gLinkReactionCooldown = 0;
  }
}
