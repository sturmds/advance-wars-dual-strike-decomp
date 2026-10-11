#include "types.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/win_lose.h"

extern MenuState gMenuState;
extern TaskScriptCmd data_02159ce0[];

/* On the results screen, starts the level-up task if any CO of side A
 * (two per player, up to six) gained a level. The task gets the count and
 * a bit mask of those COs. */
void CoExp_StartLevelUpTask(void)
{
    int mask = 0;
    int count = 0;
    int i;
    int limit = data_022a7a88.countA * 2;
    CoExpLevelUpTask *task;

    for (i = 0; i < limit; i++) {
        if (i >= 6) {
            break;
        }
        if (gMenuState.coExp.exp[i].levelUpFrame != 0) {
            mask |= 1 << i;
            count++;
        }
    }
    if (count == 0) {
        return;
    }
    task = (CoExpLevelUpTask *)Task_Create(data_02159ce0, TASK_LIST(3));
    task->mask = mask;
    task->count = count;
}
