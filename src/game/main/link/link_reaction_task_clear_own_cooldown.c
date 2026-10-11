#include "types.h"
#include "game/link_reaction_task.h"

int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
extern u16 gLinkReactionCooldown;

/* gLinkReactionCooldown is the 200 frame wait that LinkReaction_StartCooldown sets after a send. */
void LinkReactionTask_ClearOwnCooldown(LinkReactionTask *task)
{
    if (task->slot == Link_FindSlotOfPlayer(func_0207bd6c())) {
        gLinkReactionCooldown = 0;
    }
}
