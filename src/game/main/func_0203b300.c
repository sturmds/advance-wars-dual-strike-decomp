#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215a690[];

void func_0203b300(Task* parent) { Task_CreateChild(data_0215a690, parent); }
