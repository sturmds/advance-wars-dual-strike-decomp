#include "types.h"
#include "game/task.h"
#include "game/unlocks.h"

int Link_IsActive(void);
extern const TaskScriptCmd data_021618e0[];

/* Plays the script of data_021618e0 as a child task once (game flag 56),
 * unless Link_IsActive returns 1. */
void CombatIntro_StartThird(Task *parent)
{
    if (Link_IsActive() == 1) {
        return;
    }
    if (GameFlag_IsSet(56)) {
        return;
    }
    GameFlag_Set(56, 1);
    Task_CreateChild(data_021618e0, parent);
}
