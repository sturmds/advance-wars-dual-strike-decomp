#include "types.h"
#include "game/task.h"

int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
void Task_SetDestroyCallback(Task *task, void (*onDestroy)(Task *task));

/* Called for each reaction task (LinkReaction_StartCooldown). Drops the destroy callback of this
 * player's own reaction. */
void LinkReaction_ClearOwnDestroyCallback(Task *task)
{
    if (task->unk_2e != Link_FindSlotOfPlayer(func_0207bd6c())) {
        return;
    }
    Task_SetDestroyCallback(task, NULL);
}
