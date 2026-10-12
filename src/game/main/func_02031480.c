#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_02159f80[];

void func_02031480(Task* parent) { Task_CreateChild(data_02159f80, parent); }
