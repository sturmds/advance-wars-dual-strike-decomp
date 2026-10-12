#include "types.h"
#include "game/task.h"

int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
void Task_SetDestroyCallback(Task* task, void (*onDestroy)(Task* task));

/* Called for each reaction task (LinkReaction_StartCooldown). Drops the destroy callback of this
 * player's own reaction. */
void LinkReaction_ClearOwnDestroyCallback(Task* task) {
  if (task->unk_2e != Link_FindSlotOfPlayer(Link_GetOwnPlayer())) {
    return;
  }
  Task_SetDestroyCallback(task, NULL);
}
