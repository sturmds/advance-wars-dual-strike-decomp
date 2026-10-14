#include "types.h"
#include "game/task.h"

s8 Task_DestroyByScript(const void* descriptor);
extern TaskScriptCmd data_02158838[];
extern TaskScriptCmd data_02158850[];

void func_02021b80(void) {
  Task_DestroyByScript(data_02158838);
  Task_DestroyByScript(data_02158850);
}
