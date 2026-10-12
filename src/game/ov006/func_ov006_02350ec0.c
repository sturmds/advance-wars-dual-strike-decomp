#include "types.h"
#include "game/task.h"

extern int data_0229eae4;
extern const TaskScriptCmd data_ov006_0236a1f0[];

void func_ov006_02350ec0(void) {
  data_0229eae4 = 1;
  Task_Create(data_ov006_0236a1f0, TASK_LIST(3));
}
