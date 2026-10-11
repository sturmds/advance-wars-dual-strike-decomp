#include "types.h"

s8 Task_DestroyByScript(const void *descriptor);
void func_01ff9c48(const void *descriptor);
extern u8 data_02168318[];
extern u8 MainLoop_MapFrame[];

void func_020dc71c(void)
{
    Task_DestroyByScript(data_02168318);
    func_01ff9c48(MainLoop_MapFrame);
}
