#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0216a52c[];

Task* func_020fc234(void) {
  MenuButtons_DisableTouch();
  return Task_Create(data_0216a52c, TASK_LIST(3));
}
