#include "types.h"
#include "game/ov005_task.h"

void Mem_Free(void* block);

void func_0202cbfc(Ov005Task* task) { Mem_Free(task->work); }
