#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0216176c[];

void func_0208b988(Task* parent) { Task_CreateChild(data_0216176c, parent); }
