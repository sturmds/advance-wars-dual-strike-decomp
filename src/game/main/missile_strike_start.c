#include "types.h"
#include "game/missile_strike_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02169088[];

/* Starts the missile strike task (list 3) for tile (tileX, tileY). The third
 * argument is overwritten by MissileStrike_Init, the fourth is not read;
 * func_020f1f38 passes 2 and 80. */
void MissileStrike_Start(int tileX, int tileY, int fallHeight, int arg58)
{
    MissileStrikeTask *task = (MissileStrikeTask *)Task_Create(data_02169088, TASK_LIST(3));

    task->tileX = tileX;
    task->tileY = tileY;
    task->unk_3c = 8;
    task->fallHeight = fallHeight;
    task->unk_58 = arg58;
}
