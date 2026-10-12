#include "types.h"
#include "game/ov005_task.h"

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);

extern void* data_ov005_0236bb90;

/* First step of a screen with a 0x97e0-byte work block, cleared to zero. */
void Ov005Screen_AllocLargeWork(Ov005Task* task) {
  void* work;

  task->work = Mem_Alloc(0x97e0);
  work = task->work;
  data_ov005_0236bb90 = work;
  Mem_Zero32(0, work, 0x97e0);
}
