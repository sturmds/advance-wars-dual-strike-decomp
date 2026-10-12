#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a758[];

void func_020fd8a4(Task* parent) { Task_CreateChild(data_0216a758, parent); }
