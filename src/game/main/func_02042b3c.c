#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215a960[];

void func_02042b3c(void) {
  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  Task_Create(data_0215a960, TASK_LIST(3));
}
