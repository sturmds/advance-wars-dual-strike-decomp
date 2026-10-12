#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215af10[];

void func_02048238(Task* parent) { Task_CreateChild(data_0215af10, parent); }
