#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215bb68[];

void func_0205102c(void* parentOrList) {
  Task_Create(data_0215bb68, parentOrList);
}
