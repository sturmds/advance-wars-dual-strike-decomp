#include "types.h"
#include "game/task.h"

int Link_IsActive(void);
extern const TaskScriptCmd data_02160534[];

void func_02080684(Task* parent) {
  if (Link_IsActive() == 1) {
    Task_CreateChild(data_02160534, parent);
  }
}
