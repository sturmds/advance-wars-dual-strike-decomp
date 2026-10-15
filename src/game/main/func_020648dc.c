#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215cfa8[];

Task* func_020648dc(void) {
  MenuButtons_DisableTouch();
  return Task_Create(data_0215cfa8, TASK_LIST(3));
}
