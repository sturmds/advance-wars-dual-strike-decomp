#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb84.h"

void* Mem_Alloc(u32 size);

/* also declared in src/game/ov005/func_ov005_02355224.c and 2 more */
Unk_ov005_0236bb84* data_ov005_0236bb84;

void func_ov005_023551f8(Ov005Task* task) {
  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bb84));
  data_ov005_0236bb84 = task->work;
}
