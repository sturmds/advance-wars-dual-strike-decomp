#include "types.h"
#include "game/task.h"

int Link_IsActive(void);
extern const TaskScriptCmd data_0216065c[];

void func_020811e4(Task* parent) {
  if (Link_IsActive() == 1) {
    Task_CreateChild(data_0216065c, parent);
  }
}
