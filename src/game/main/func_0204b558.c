#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215b164[];

void func_0204b558(void* parentOrList) {
  Task_Create(data_0215b164, parentOrList);
}
