#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215ccb0[];

void func_020654dc(void) {
  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  Task_Create(data_0215ccb0, TASK_LIST(3));
}
