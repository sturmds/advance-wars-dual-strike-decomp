#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a730[];

void func_020fd88c(Task* parent) { Task_CreateChild(data_0216a730, parent); }
