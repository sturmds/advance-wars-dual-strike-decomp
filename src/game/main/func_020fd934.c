#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a848[];

void func_020fd934(Task* parent) { Task_CreateChild(data_0216a848, parent); }
