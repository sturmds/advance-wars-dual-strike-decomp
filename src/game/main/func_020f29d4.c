#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_021695e8[];

void func_020f29d4(void* parentOrList) {
  Task_Create(data_021695e8, parentOrList);
}
