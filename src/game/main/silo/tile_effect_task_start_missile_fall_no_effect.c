#include "types.h"
#include "game/task.h"
#include "game/tile_effect_task.h"

void SiloMissileFall_Start(int x, int y, int damage, int effect, Task *parent);

/* Same as TileEffectTask_StartMissileFall but with effect 0. */
void TileEffectTask_StartMissileFallNoEffect(TileEffectTask *task)
{
    SiloMissileFall_Start(task->x, task->y, task->unk_4a, 0, (Task *)task);
}
