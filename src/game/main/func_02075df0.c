#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215fc0c[];

void func_02075df0(Task* parent) { Task_CreateChild(data_0215fc0c, parent); }
