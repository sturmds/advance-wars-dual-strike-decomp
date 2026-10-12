#include "types.h"
#include "game/ov005_task.h"

void* Mem_Alloc(u32 size);

extern void* data_ov005_0236bb8c;

void func_ov005_02358928(Ov005Task* task) {
  task->work = Mem_Alloc(0x18);
  data_ov005_0236bb8c = task->work;
}
