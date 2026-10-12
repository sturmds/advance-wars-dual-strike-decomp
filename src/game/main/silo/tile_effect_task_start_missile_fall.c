#include "types.h"
#include "game/task.h"
#include "game/tile_effect_task.h"

void SiloMissileFall_Start(int x, int y, int damage, int effect, Task* parent);

/* Script step of a tile effect task: starts a missile fall on its tile with
 * the task's effect id, with the task as parent. */
void TileEffectTask_StartMissileFall(TileEffectTask* task) {
  SiloMissileFall_Start(task->x, task->y, task->unk_4a, task->effect,
                        (Task*)task);
}
