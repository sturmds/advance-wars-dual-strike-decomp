#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a998[];

void func_020ffaec(Task* parent) { Task_CreateChild(data_0216a998, parent); }
