#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_021698d0[];

void func_020f43f4(Task* parent) { Task_CreateChild(data_021698d0, parent); }
