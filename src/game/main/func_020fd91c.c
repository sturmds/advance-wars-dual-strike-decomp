#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a820[];

void func_020fd91c(Task* parent) { Task_CreateChild(data_0216a820, parent); }
