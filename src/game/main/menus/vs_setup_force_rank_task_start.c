#include "types.h"
#include "game/Unk_02080440.h"
#include "game/force_rank_task.h"
#include "game/menu_state.h"

void ForceRankScreen_Start(int co, int mode, int set, int taskList);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern MenuState gMenuState;

/* Script step of the Force Rank screen task. Unless a cancel is pending
 * (state 4), sets state 2 and starts the Force Rank screen for the task's CO
 * and set, as a child of this task. The (s16) cast is needed to match. */
void VsSetup_ForceRankTaskStart(ForceRankTask *task)
{
    if (gMenuState.setup.unk_2ea == 4) {
        return;
    }
    gMenuState.setup.unk_2ea = 2;
    ForceRankScreen_Start(task->co, data_027c027c->unk_48[data_027c0284]->gameMode, task->forceRankSet, (int)task);
}
