#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a708[];

void func_020fd874(Task* parent) { Task_CreateChild(data_0216a708, parent); }
