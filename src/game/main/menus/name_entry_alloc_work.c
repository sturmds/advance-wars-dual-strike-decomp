#include "types.h"
#include "game/Unk_022a7ea8.h"

#define NAME_ENTRY_WORK_SIZE 0x4f4

void *Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);

extern NameEntry *data_022a7ea8;

/* First step of the name entry screen: allocates and clears the work block. */
void func_02100838(NameEntryTask *task)
{
    task->work = Mem_Alloc(NAME_ENTRY_WORK_SIZE);
    data_022a7ea8 = task->work;
    Mem_Zero32(0, data_022a7ea8, NAME_ENTRY_WORK_SIZE);
}
