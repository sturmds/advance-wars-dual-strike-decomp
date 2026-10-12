#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168e18[];

void TileEffectTask_Create(int x, int y, int effect, Task* parent);

/* Starts tile effect 25 at tile (x, y) under `parent`, in a new task at top level 3. */
void func_020eed64(int x, int y, Task* parent) {
  Task_Create(data_02168e18, TASK_LIST(3));
  TileEffectTask_Create(x, y, 25, parent);
}
