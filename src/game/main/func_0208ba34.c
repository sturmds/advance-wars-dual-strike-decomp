#include "types.h"
#include "game/task.h"

void MenuButtons_DisableTouch(void);
extern const TaskScriptCmd data_0216179c[];

Task* func_0208ba34(void) {
  MenuButtons_DisableTouch();
  return Task_Create(data_0216179c, TASK_LIST(3));
}
