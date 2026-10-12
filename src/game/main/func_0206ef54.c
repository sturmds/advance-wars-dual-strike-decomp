#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215d400[];

void func_0206ef54(void) {
  MenuButtons_DisableTouch();
  MenuButtons_CloseAll();
  Task_Create(data_0215d400, TASK_LIST(3));
}
