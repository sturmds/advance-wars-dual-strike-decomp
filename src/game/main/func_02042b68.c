#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215a940[];

void func_02042b68(void) {
  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  Task_Create(data_0215a940, TASK_LIST(3));
}
