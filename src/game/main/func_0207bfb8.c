#include "types.h"
#include "game/task.h"

extern u8* data_0215fed0;
extern const TaskScriptCmd data_02160044[];

void func_0207bfb8(void) {
  *data_0215fed0 = 1;
  Task_Create(data_02160044, TASK_LIST(3));
}
