#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215ad6c[];

void func_020458a8(Task* parent) { Task_CreateChild(data_0215ad6c, parent); }
