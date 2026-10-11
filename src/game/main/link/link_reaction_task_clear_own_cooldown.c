#include "types.h"
#include "game/link_reaction_task.h"

int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
extern u16 data_0229da64;

/* data_0229da64 is the 200 frame wait that func_02080844 sets after a send. */
void LinkReactionTask_ClearOwnCooldown(LinkReactionTask *task)
{
    if (task->slot == Link_FindSlotOfPlayer(func_0207bd6c())) {
        data_0229da64 = 0;
    }
}
