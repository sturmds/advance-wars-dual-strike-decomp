#include "types.h"
#include "game/task.h"

void Palette_Load(const void *src, u32 offset, u32 size);
void *Task_Create(const void *script, void *parentOrList);
extern const u16 data_02149280[];
extern const u32 data_02160bb8[];

void func_02082a8c(int palette)
{
    Task *task;

     
    Palette_Load(data_02149280, ((u32)palette << 21) >> 16, 32);
    task = Task_Create(data_02160bb8, (void *)3);
    task->unk_52 = palette;
}
