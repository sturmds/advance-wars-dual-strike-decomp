#include "types.h"
#include "game/ov005_task.h"

void *Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);
extern void *data_ov005_0236bbb0;

void func_ov005_02366eec(Ov005Task *task)
{
    void *work;

    task->work = Mem_Alloc(0x1d68);
    work = task->work;
    data_ov005_0236bbb0 = work;
    Mem_Zero32(0, work, 0x1d68);
}
