#include "types.h"
#include "game/ov005_task.h"

void Mem_Free(void* block);

void func_ov005_0235788c(Ov005Task* task) { Mem_Free(task->work); }
