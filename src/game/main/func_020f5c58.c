#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_02169df8[];

void func_020f5c58(Task* parent) { Task_CreateChild(data_02169df8, parent); }
