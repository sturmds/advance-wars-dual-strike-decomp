#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168e18[];
void TileEffectTask_Create(int x, int y, int effect, Task* parent);

void func_020eed28(int x, int y, Task* parent) {
  Task_Create(data_02168e18, TASK_LIST(3));
  TileEffectTask_Create(x, y, 26, parent);
}
