#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215b22c[];

void func_02048d98(Task* parent) { Task_CreateChild(data_0215b22c, parent); }
