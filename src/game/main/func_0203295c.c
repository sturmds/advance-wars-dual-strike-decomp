#include "types.h"
#include "game/task.h"

void func_02026868(int);
extern const TaskScriptCmd data_02159c90[];

Task* func_0203295c(void* parentOrList) {
  func_02026868(9);
  return Task_Create(data_02159c90, parentOrList);
}
