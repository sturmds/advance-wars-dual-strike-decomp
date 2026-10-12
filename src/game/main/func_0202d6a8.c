#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_02159b44[];

void func_0202d6a8(Task* parent) { Task_CreateChild(data_02159b44, parent); }
