#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"

void* Mem_Alloc(u32 size);

/* also declared in src/game/ov005/func_ov005_023634c8.c and 6 more */
void* data_ov005_0236bba8;

void func_ov005_02362618(Ov005Task* task) {
  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bba8));
  data_ov005_0236bba8 = task->work;
}
