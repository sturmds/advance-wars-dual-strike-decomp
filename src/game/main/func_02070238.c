#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215d664[];

void func_02070238(Task* parent) { Task_CreateChild(data_0215d664, parent); }
