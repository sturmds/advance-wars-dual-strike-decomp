#include "types.h"
#include "game/task.h"
#include "game/unlocks.h"

extern const TaskScriptCmd data_0215a910[];

void func_02041864(Task* parent) {
  if (GameFlag_IsSet(58)) {
    return;
  }
  GameFlag_Set(58, 1);
  Task_CreateChild(data_0215a910, parent);
}
