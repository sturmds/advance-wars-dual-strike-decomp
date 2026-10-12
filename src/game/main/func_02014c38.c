#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_02158354[];

void func_02014c38(void* parentOrList) {
  Task_Create(data_02158354, parentOrList);
}
