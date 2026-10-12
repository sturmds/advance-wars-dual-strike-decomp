#include "types.h"
#include "game/link_reaction_task.h"

int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
extern u16 gLinkReactionCooldown;

/* gLinkReactionCooldown is the 200 frame wait that LinkReaction_StartCooldown sets after a send. */
void LinkReactionTask_ClearOwnCooldown(LinkReactionTask* task) {
  if (task->slot == Link_FindSlotOfPlayer(Link_GetOwnPlayer())) {
    gLinkReactionCooldown = 0;
  }
}
