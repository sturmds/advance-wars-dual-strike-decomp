#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"

void* Mem_Alloc(u32 size);

extern Unk_ov005_0236bb80* data_ov005_0236bb80;

void func_ov005_02352790(Ov005Task* task) {
  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bb80));
  data_ov005_0236bb80 = task->work;
}
