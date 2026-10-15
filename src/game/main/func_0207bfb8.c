#include "types.h"
#include "game/link_menu_state.h"
#include "game/task.h"

extern const TaskScriptCmd data_02160044[];

void func_0207bfb8(void) {
  data_0215fed0->view = 1;
  Task_Create(data_02160044, TASK_LIST(3));
}
