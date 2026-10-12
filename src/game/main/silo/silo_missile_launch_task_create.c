#include "types.h"
#include "game/silo_missile_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168e48[];

/* Starts the launching missile task (script data_02168e48) for tile (x, y)
 * under `parent`. The sprite uses tile `tile` and palette `palette`. */
void SiloMissileLaunch_CreateTask(int x, int y, int tile, int palette,
                                  Task* parent) {
  SiloMissileTask* task =
      (SiloMissileTask*)Task_CreateChild(data_02168e48, parent);

  task->x = x;
  task->y = y;
  task->spriteAttr = tile | palette << 12;
}
