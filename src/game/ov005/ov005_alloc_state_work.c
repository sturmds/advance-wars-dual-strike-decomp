#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba4.h"

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);

extern Unk_ov005_0236bba4* data_ov005_0236bba4;

void Ov005_AllocStateWork(Ov005Task* task) {
  Unk_ov005_0236bba4* work;

  task->work = Mem_Alloc(sizeof(Unk_ov005_0236bba4));
  work = task->work;
  data_ov005_0236bba4 = work;
  Mem_Zero32(0, work, sizeof(Unk_ov005_0236bba4));
}
