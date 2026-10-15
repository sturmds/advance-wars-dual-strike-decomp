#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215f8ac[];

Task* func_02072a28(void) {
  MenuButtons_DisableTouch();
  return Task_Create(data_0215f8ac, TASK_LIST(3));
}
