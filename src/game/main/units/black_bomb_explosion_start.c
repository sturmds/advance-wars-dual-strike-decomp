#include "types.h"
#include "game/black_bomb_blast_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168f90[];

/* Starts the Black Bomb explosion task for a bomb at (bombX, bombY) that
 * blasts around (tileX, tileY). parentOrList is a top-level list number
 * (below TASK_LEVELS) or the parent task. */
void BlackBombExplosion_Start(int bombX, int bombY, int tileX, int tileY, void *parentOrList)
{
    BlackBombExplosionTask *task;

    if ((int)parentOrList < TASK_LEVELS) {
        task = (BlackBombExplosionTask *)Task_Create(data_02168f90, parentOrList);
    } else {
        task = (BlackBombExplosionTask *)Task_CreateChild(data_02168f90, parentOrList);
    }
    task->bombX = bombX;
    task->bombY = bombY;
    task->tileX = tileX;
    task->tileY = tileY;
    task->damage = 50;
}
