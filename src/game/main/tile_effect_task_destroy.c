#include "types.h"
#include "game/tile_effect_task.h"

void func_0202244c(void *handle);

void TileEffectTask_Destroy(TileEffectTask *task)
{
    func_0202244c(task->sprite);
}
