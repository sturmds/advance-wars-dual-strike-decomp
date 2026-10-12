#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216a4ec[];

void func_020fc160(Task* parent) { Task_CreateChild(data_0216a4ec, parent); }
