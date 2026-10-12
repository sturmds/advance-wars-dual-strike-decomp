#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a7d0[];

void func_020fd8ec(Task* parent) { Task_CreateChild(data_0216a7d0, parent); }
