#include "types.h"
#include "game/task.h"

int Link_IsActive(void);
extern const TaskScriptCmd data_02160624[];

void func_020811b4(Task* parent) {
  if (Link_IsActive() == 1) {
    Task_CreateChild(data_02160624, parent);
  }
}
