#include "types.h"
#include "game/task.h"

s8 Task_DestroyByScript(const void* descriptor);
extern TaskScriptCmd data_02159830[];
extern TaskScriptCmd data_02159870[];

void func_0202a7c0(void) {
  Task_DestroyByScript(data_02159830);
  Task_DestroyByScript(data_02159870);
}
