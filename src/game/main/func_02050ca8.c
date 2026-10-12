#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215bba8[];

void func_02050ca8(void* parentOrList) {
  Task_Create(data_0215bba8, parentOrList);
}
