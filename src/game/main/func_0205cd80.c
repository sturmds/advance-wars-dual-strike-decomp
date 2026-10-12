#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215c9a8[];

void func_0205cd80(Task* parent) { Task_CreateChild(data_0215c9a8, parent); }
