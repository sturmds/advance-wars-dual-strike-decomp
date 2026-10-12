#include "types.h"
#include "game/task.h"

void Mem_Free(void* ptr);

/* Last step of the range spread script: frees the dot array. */
void RangeSpread_Destroy(Task* task) { Mem_Free(task->unk_4c); }
