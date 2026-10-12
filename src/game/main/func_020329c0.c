#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_02159c78[];

void func_020329c0(Task* parent) { Task_CreateChild(data_02159c78, parent); }
