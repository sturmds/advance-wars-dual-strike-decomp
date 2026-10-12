#include "types.h"
#include "game/task.h"

void CoSelect_ResetToUnlockedCos(void);
extern const TaskScriptCmd data_0216d5fc[];

Task* func_02103310(void) {
  CoSelect_ResetToUnlockedCos();
  return Task_Create(data_0216d5fc, TASK_LIST(3));
}
