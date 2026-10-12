#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_021695b8[];

void func_020f1aac(void* parentOrList) {
  Task_Create(data_021695b8, parentOrList);
}
