#include "types.h"

extern int data_022acf04;
void MI_CpuFill8(int, int, int, void *);

void func_0211e1f4(int a0)
{
    data_022acf04 = a0;
    MI_CpuFill8(a0, 0, 0x21c, &data_022acf04);
}
