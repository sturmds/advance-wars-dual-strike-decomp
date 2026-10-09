#include "types.h"

void func_0208be18(int arg);
void *Task_Create(const void *script, void *parent);
extern u8 data_0216d6d4[];

void func_02102bc4(void)
{
    func_0208be18(1);
    Task_Create(data_0216d6d4, (void *)3);
}
