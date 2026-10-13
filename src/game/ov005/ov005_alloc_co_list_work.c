#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba0.h"

void* Mem_Alloc(u32 size);

/* also declared in src/game/ov005/func_ov005_0235cee4.c and 6 more */
Unk_ov005_0236bba0* data_ov005_0236bba0;

void Ov005_AllocCoListWork(Ov005Task* task) {
  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bba0));
  data_ov005_0236bba0 = task->work;
}
