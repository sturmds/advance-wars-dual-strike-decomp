#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbb0.h"

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);

/* also declared in src/game/ov005/func_ov005_0236701c.c and 9 more */
Unk_ov005_0236bbb0* data_ov005_0236bbb0;

void Ov005_AllocShopWork(Ov005Task* task) {
  Unk_ov005_0236bbb0* work;

  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bbb0));
  work = task->work;
  data_ov005_0236bbb0 = work;
  Mem_Zero32(0, work, sizeof(Unk_ov005_0236bbb0));
}
