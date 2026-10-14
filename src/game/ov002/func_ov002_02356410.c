#include "types.h"
#include "game/task.h"

s8 Task_DestroyByScript(const void* descriptor);
extern TaskScriptCmd data_ov002_023695e0[];
extern TaskScriptCmd data_ov002_023695b8[];

void func_ov002_02356410(void) {
  Task_DestroyByScript(data_ov002_023695e0);
  Task_DestroyByScript(data_ov002_023695b8);
}
