#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a780[];

void func_020fd8bc(Task* parent) { Task_CreateChild(data_0216a780, parent); }
