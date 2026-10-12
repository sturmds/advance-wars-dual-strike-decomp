#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215c444[];

void func_02055c94(Task* parent) { Task_CreateChild(data_0215c444, parent); }
