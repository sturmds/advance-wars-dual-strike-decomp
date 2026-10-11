#include "types.h"
#include "game/silo_missile_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168e78[];

/* Starts the falling missile task (script data_02168e78) for tile (x, y)
 * under `parent`. The sprite uses tile `tile` and palette `palette`. damage
 * and effect are kept for SiloMissile_Explode. */
void SiloMissileFall_CreateTask(int x, int y, int tile, int palette, int damage, int effect, Task *parent)
{
    SiloMissileTask *task = (SiloMissileTask *)Task_CreateChild(data_02168e78, parent);

    task->x = x;
    task->y = y;
    task->spriteAttr = tile | palette << 12;
    task->damage = damage;
    task->effect = effect;
}
