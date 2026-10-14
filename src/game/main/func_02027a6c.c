#include "types.h"
#include "game/task.h"

s8 Task_DestroyByScript(const void* descriptor);
extern TaskScriptCmd data_02159134[];
extern TaskScriptCmd data_02159114[];

void func_02027a6c(void) {
  Task_DestroyByScript(data_02159134);
  Task_DestroyByScript(data_02159114);
}
