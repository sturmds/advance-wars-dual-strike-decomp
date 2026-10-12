#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215c4f4[];

void func_02055cac(Task* parent) { Task_CreateChild(data_0215c4f4, parent); }
