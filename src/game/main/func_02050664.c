#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215bc10[];

void func_02050664(Task* parent) { Task_CreateChild(data_0215bc10, parent); }
