#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215b06c[];

void func_0204c11c(void* parentOrList) {
  Task_Create(data_0215b06c, parentOrList);
}
