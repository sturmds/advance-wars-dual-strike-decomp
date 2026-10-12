#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_021615d0[];

void func_0208922c(Task* parent) { Task_CreateChild(data_021615d0, parent); }
