#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_02159ff8[];

void func_02032b28(void* parentOrList) {
  Task_Create(data_02159ff8, parentOrList);
}
