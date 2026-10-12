#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a8b0[];

void func_020fd94c(Task* parent) { Task_CreateChild(data_0216a8b0, parent); }
